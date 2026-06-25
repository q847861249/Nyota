// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NyotaHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"
#include "Character/BasePlayerController.h"
#include "Character/BasePlayerState.h"
#include "Character/NyotaPawnData.h"
#include "Character/NyotaPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GA/CustomAbilitySystemComponent.h"
#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"
#include "GameplayTags/GameTags.h"
#include "Input/NyotaInputComponent.h"
#include "Misc/UObjectToken.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NyotaHeroComponent)

namespace NyotaHero
{
static constexpr float LookYawRate = 300.0f;
static constexpr float LookPitchRate = 165.0f;
}; // namespace NyotaHero

const FName UNyotaHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UNyotaHeroComponent::NAME_ActorFeatureName("Hero");

bool UNyotaHeroComponent::CanChangeInitState(
    UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
) const
{
    /*
    [未初始化] ──→ Spawned ──→ DataAvailable ──→ DataInitialized ──→ GameplayReady
    │              │              │                   │                  │
    │   有真实Pawn  │   PlayerState │  PS + PawnExt就绪  │  (无额外条件)     │
    │   即可进入    │   + Controller│                    │                  │
    │              │   + Input组件 │                    │                  │
    */

    check(Manager);

    APawn *Pawn = GetPawn<APawn>();

    if (!CurrentState.IsValid() && DesiredState == Nyota::InitState_Spawned)
    {
        // 只要持有一个真实的 Pawn，就允许推进到 Spawned 状态
        if (Pawn)
        {
            return true;
        }
    }

    if (CurrentState == Nyota::InitState_Spawned && DesiredState == Nyota::InitState_DataAvailable)
    {
        // 必须有 PlayerState。
        if (!GetPlayerState<ABasePlayerState>())
        {
            return false;
        }

        // 在服务端（Authority）或本地自主代理（Autonomous）上，需要等待一个已持有 PlayerState 所有权的 Controller。
        if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
        {
            AController *Controller = GetController<AController>();

            const bool bHasControllerPairedWithPS = Controller != nullptr && Controller->PlayerState != nullptr &&
                                                    Controller->PlayerState->GetOwner() == Controller;

            if (!bHasControllerPairedWithPS)
            {
                return false;
            }
        }

        const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
        const bool bIsBot = Pawn->IsBotControlled();

        if (bIsLocallyControlled && !bIsBot)
        {
            ABasePlayerController *NyotaPC = GetController<ABasePlayerController>();

            // 当本地控制时，必须有 InputComponent 和 LocalPlayer。
            if (!Pawn->InputComponent || !NyotaPC || !NyotaPC->GetLocalPlayer())
            {
                return false;
            }
        }

        return true;
    }

    if (CurrentState == Nyota::InitState_DataAvailable && DesiredState == Nyota::InitState_DataInitialized)
    {
        // 等待 PlayerState 和 PawnExtensionComponent 就绪
        ABasePlayerState *NyotaPS = GetPlayerState<ABasePlayerState>();
        const bool bRes = Manager->HasFeatureReachedInitState(
            Pawn, UNyotaPawnExtensionComponent::NAME_ActorFeatureName, Nyota::InitState_DataInitialized
        );

        return NyotaPS && bRes;
    }

    if (CurrentState == Nyota::InitState_DataInitialized && DesiredState == Nyota::InitState_GameplayReady)
    {
        // TODO 添加技能初始化检查？
        return true;
    }

    return false;
}

void UNyotaHeroComponent::HandleChangeInitState(
    UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
)
{
    if (CurrentState == Nyota::InitState_DataAvailable && DesiredState == Nyota::InitState_DataInitialized)
    {
        APawn *Pawn = GetPawn<APawn>();
        ABasePlayerState *NyotaPS = GetPlayerState<ABasePlayerState>();
        if (!ensure(Pawn && NyotaPS))
        {
            return;
        }

        const UNyotaPawnData *PawnData = nullptr;

        if (UNyotaPawnExtensionComponent *PawnExtComp = UNyotaPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
        {
            PawnData = PawnExtComp->GetPawnData<UNyotaPawnData>();

            // PlayerState 持有该玩家的持久数据（跨死亡和跨 Pawn 切换仍然保留的状态）。
            // 技能系统组件（ASC）和属性集（AttributeSet）都住在 PlayerState 上。
            PawnExtComp->InitializeAbilitySystem(NyotaPS->GetNyotaAbilitySystemComponent(), NyotaPS);
        }

        if (GetController<ABasePlayerController>())
        {
            if (Pawn->InputComponent != nullptr)
            {
                InitializePlayerInput(Pawn->InputComponent);
            }
        }

        // 为所有 Pawn 挂上委托，以防后续切到观战模式
        // if (PawnData)
        // {
        // }
    }
}

void UNyotaHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams &Params)
{
    if (Params.FeatureName == UNyotaPawnExtensionComponent::NAME_ActorFeatureName)
    {
        if (Params.FeatureState == Nyota::InitState_DataInitialized)
        {
            // 当 PawnExtensionComponent 表示所有其他组件都已初始化完毕，
            // 则尝试推进到下一个状态
            CheckDefaultInitialization();
        }
    }
}

void UNyotaHeroComponent::CheckDefaultInitialization()
{
    static const TArray<FGameplayTag> StateChain = {
        Nyota::InitState_Spawned,
        Nyota::InitState_DataAvailable,
        Nyota::InitState_DataInitialized,
        Nyota::InitState_GameplayReady
    };

    // 从 Spawned 状态（仅在 BeginPlay 中设置）开始，依次尝试推进整个数据初始化阶段，直到抵达 GameplayReady
    ContinueInitStateChain(StateChain);
}

void UNyotaHeroComponent::OnRegister()
{
    Super::OnRegister();

    if (!GetPawn<APawn>())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "[UNyotaHeroComponent::OnRegister] This component has been added to a blueprint whose base class is "
                "not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."
            )
        );

#if WITH_EDITOR
        if (GIsEditor)
        {
            static const FText Message = NSLOCTEXT(
                "NyotaHeroComponent",
                "NotOnPawnError",
                "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be "
                "placed on a Pawn Blueprint. This will cause a crash if you PIE!"
            );
            static const FName HeroMessageLogName = TEXT("NyotaHeroComponent");

            FMessageLog(HeroMessageLogName)
                .Error()
                ->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
                ->AddToken(FTextToken::Create(Message));

            FMessageLog(HeroMessageLogName).Open();
        }
#endif
    }
    else
    {
        // 尽早在初始化状态系统中注册，只有在这是游戏世界时才会有效
        RegisterInitStateFeature();
    }
}

void UNyotaHeroComponent::BeginPlay()
{
    Super::BeginPlay();

    // 监听 PawnExtensionComponent 的初始化状态变化
    BindOnActorInitStateChanged(UNyotaPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

    // 通知外部：我们已经完成 Spawn，然后尝试继续推进剩余初始化
    ensure(TryToChangeInitState(Nyota::InitState_Spawned));
    CheckDefaultInitialization();
}

void UNyotaHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnregisterInitStateFeature();

    Super::EndPlay(EndPlayReason);
}

UNyotaHeroComponent::UNyotaHeroComponent(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    bReadyToBindInputs = false;
}

bool UNyotaHeroComponent::IsReadyToBindInputs() const
{
    return bReadyToBindInputs;
}

void UNyotaHeroComponent::InitializePlayerInput(UInputComponent *PlayerInputComponent)
{
    check(PlayerInputComponent);

    const APawn *Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return;
    }

    const APlayerController *PlayerController = GetController<APlayerController>();
    check(PlayerController);

    const ULocalPlayer *LocalPlayer = PlayerController->GetLocalPlayer();
    check(LocalPlayer);

    UEnhancedInputLocalPlayerSubsystem *Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(Subsystem);

    Subsystem->ClearAllMappings();

    if (const UNyotaPawnExtensionComponent *PawnExtComp =
            UNyotaPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
    {
        if (const UNyotaPawnData *PawnData = PawnExtComp->GetPawnData<UNyotaPawnData>())
        {
            if (const UNyotaInputConfig *InputConfig = PawnData->InputConfig)
            {
                for (const FInputMappingContextAndPriority &Mapping : DefaultInputMappings)
                {
                    if (UInputMappingContext *IMC = Mapping.InputMapping.LoadSynchronous())
                    {
                        if (Mapping.bRegisterWithSetting)
                        {
                            if (UEnhancedInputUserSettings *Settings = Subsystem->GetUserSettings())
                            {
                                Settings->RegisterInputMappingContext(IMC);
                            }

                            FModifyContextOptions Options = {};
                            Options.bIgnoreAllPressedKeysUntilRelease = false;

                            // 实际上将配置添加到本地玩家
                            Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
                        }
                    }
                }

                UNyotaInputComponent *NyotaIC = Cast<UNyotaInputComponent>(PlayerInputComponent);
                if (ensureMsgf(
                        NyotaIC,
                        TEXT(
                            "Unexpected Input Component class! The Gameplay Abilities will not be bound to their "
                            "inputs. Change the input component to UNyotaInputComponent or a subclass of it."
                        )
                    ))
                {
                    NyotaIC->AddInputMappings(InputConfig, Subsystem);

                    TArray<uint32> BindHandles;
                    NyotaIC->BindAbilityActions(
                        InputConfig,
                        this,
                        &ThisClass::Input_AbilityInputTagPressed,
                        &ThisClass::Input_AbilityInputTagReleased,
                        BindHandles
                    );

                    NyotaIC->BindNativeAction(
                        InputConfig, Nyota::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false
                    );
                    NyotaIC->BindNativeAction(
                        InputConfig,
                        Nyota::InputTag_Look_Mouse,
                        ETriggerEvent::Triggered,
                        this,
                        &ThisClass::Input_LookMouse,
                        false
                    );
                    NyotaIC->BindNativeAction(
                        InputConfig,
                        Nyota::InputTag_Look_Stick,
                        ETriggerEvent::Triggered,
                        this,
                        &ThisClass::Input_LookStick,
                        false
                    );
                }
            }
        }
    }

    if (ensure(!bReadyToBindInputs))
    {
        bReadyToBindInputs = true;
    }

    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
        const_cast<APlayerController *>(PlayerController), NAME_BindInputsNow
    );
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
        const_cast<APawn *>(Pawn), NAME_BindInputsNow
    );
}

void UNyotaHeroComponent::Input_AbilityInputTagPressed(const FGameplayTag &InputTag)
{
    const APawn *Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return;
    }

    UNyotaPawnExtensionComponent *PawnExtComp = UNyotaPawnExtensionComponent::FindPawnExtensionComponent(Pawn);
    if (!PawnExtComp)
    {
        return;
    }

    if (UCustomAbilitySystemComponent *NyotaASC = PawnExtComp->GetNyotaAbilitySystemComponent())
    {
        NyotaASC->AbilityInputTagPressed(InputTag);
    }
}

void UNyotaHeroComponent::Input_AbilityInputTagReleased(const FGameplayTag &InputTag)
{
    const APawn *Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return;
    }

    UNyotaPawnExtensionComponent *PawnExtComp = UNyotaPawnExtensionComponent::FindPawnExtensionComponent(Pawn);
    if (!PawnExtComp)
    {
        return;
    }

    if (UCustomAbilitySystemComponent *NyotaASC = PawnExtComp->GetNyotaAbilitySystemComponent())
    {
        NyotaASC->AbilityInputTagReleased(InputTag);
    }
}

void UNyotaHeroComponent::Input_Move(const FInputActionValue &InputActionValue)
{
    APawn *Pawn = GetPawn<APawn>();
    AController *Controller = Pawn->GetController();

    if (!Controller)
    {
        return;
    }

    const FVector2D Axis = InputActionValue.Get<FVector2D>();
    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, ControlRotation.Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    Pawn->AddMovementInput(ForwardDirection, Axis.Y);
    Pawn->AddMovementInput(RightDirection, Axis.X);
}

void UNyotaHeroComponent::Input_LookMouse(const FInputActionValue &InputActionValue)
{
    APawn *Pawn = GetPawn<APawn>();
    AController *Controller = Pawn->GetController();

    if (!Controller)
    {
        return;
    }

    const FVector2D LookVector2D = InputActionValue.Get<FVector2D>();

    Pawn->AddControllerYawInput(LookVector2D.X);
    Pawn->AddControllerPitchInput(LookVector2D.Y);
}

void UNyotaHeroComponent::Input_LookStick(const FInputActionValue &InputActionValue)
{
    APawn *Pawn = GetPawn<APawn>();

    if (!Pawn)
    {
        return;
    }

    const FVector2D Value = InputActionValue.Get<FVector2D>();

    const UWorld *World = GetWorld();
    check(World);

    if (Value.X != 0.0f)
    {
        Pawn->AddControllerYawInput(Value.X * NyotaHero::LookYawRate * World->GetDeltaSeconds());
    }

    if (Value.Y != 0.0f)
    {
        Pawn->AddControllerPitchInput(Value.Y * NyotaHero::LookPitchRate * World->GetDeltaSeconds());
    }
}
