// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NyotaPawnExtensionComponent.h"

#include "Character/NyotaPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "AbilitySystem/NyotaAbilitySystemComponent.h"
#include "GameplayTags/GameTags.h"
#include "Net/UnrealNetwork.h"

const FName UNyotaPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UNyotaPawnExtensionComponent::UNyotaPawnExtensionComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);

    PawnData = nullptr;
    AbilitySystemComponent = nullptr;
}

UNyotaPawnExtensionComponent *UNyotaPawnExtensionComponent::FindPawnExtensionComponent(const AActor *Actor)
{
    return Actor ? Actor->FindComponentByClass<UNyotaPawnExtensionComponent>() : nullptr;
}

void UNyotaPawnExtensionComponent::InitializeAbilitySystem(UNyotaAbilitySystemComponent *InASC, AActor *InOwnerActor)
{
    check(InASC);
    check(InOwnerActor);

    // AbilitySystemComponent 没有改变
    if (AbilitySystemComponent == InASC)
    {
        return;
    }

    // 清除旧的 AbilitySystemComponent
    if (AbilitySystemComponent)
    {
        UninitializeAbilitySystem();
    }

    APawn *Pawn = GetPawnChecked<APawn>();
    AActor *ExistingAvatar = InASC->GetAvatarActor();

    UE_LOG(
        LogTemp,
        Verbose,
        TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "),
        *GetNameSafe(InASC),
        *GetNameSafe(Pawn),
        *GetNameSafe(InOwnerActor),
        *GetNameSafe(ExistingAvatar)
    );

    if (ExistingAvatar && ExistingAvatar != Pawn)
    {
        UE_LOG(LogTemp, Log, TEXT("Existing avatar (authority=%d)"), ExistingAvatar->HasAuthority() ? 1 : 0);

        // 当前已经有一个 Pawn 正在作为 ASC 的 Avatar，因此我们需要将它移除（替换掉）。
        // 这种情况可能发生在网络延迟较高的客户端上：新的 Pawn 已经生成并被 Possess，但旧的死亡 Pawn
        // 还没有被销毁或移除。
        ensure(!ExistingAvatar->HasAuthority());

        if (UNyotaPawnExtensionComponent *OtherExtensionComponent = FindPawnExtensionComponent(ExistingAvatar))
        {
            OtherExtensionComponent->UninitializeAbilitySystem();
        }
    }

    AbilitySystemComponent = InASC;
    AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

    if (ensure(PawnData))
    {
        InASC->SetTagRelationshipMapping(PawnData->TagRelationshipMapping);
    }

    OnAbilitySystemInitialized.Broadcast();
}

void UNyotaPawnExtensionComponent::UninitializeAbilitySystem()
{
    if (!AbilitySystemComponent)
    {
        return;
    }

    if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
    {
        FGameplayTagContainer AbilityTypesToIgnore;
        AbilityTypesToIgnore.AddTag(Nyota::Ability_Behavior_SurvivesDeath);

        AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
        AbilitySystemComponent->ClearAbilityInput();
        AbilitySystemComponent->RemoveAllGameplayCues();

        if (AbilitySystemComponent->GetOwnerActor() != nullptr)
        {
            AbilitySystemComponent->SetAvatarActor(nullptr);
        }
        else
        {
            // If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
            AbilitySystemComponent->ClearActorInfo();
        }

        OnAbilitySystemUninitialized.Broadcast();
    }

    AbilitySystemComponent = nullptr;
}

void UNyotaPawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(
    FSimpleMulticastDelegate::FDelegate Delegate
)
{
    if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
    {
        OnAbilitySystemInitialized.Add(Delegate);
    }

    if (AbilitySystemComponent)
    {
        Delegate.Execute();
    }
}

void UNyotaPawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
    if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
    {
        OnAbilitySystemUninitialized.Add(Delegate);
    }
}

FName UNyotaPawnExtensionComponent::GetFeatureName() const
{
    return NAME_ActorFeatureName;
}

bool UNyotaPawnExtensionComponent::CanChangeInitState(
    UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
) const
{
    check(Manager);

    APawn *Pawn = GetPawn<APawn>();

    // → Spawned：只要有 Pawn
    if (!CurrentState.IsValid() && DesiredState == Nyota::InitState_Spawned)
    {
        if (Pawn)
        {
            return true;
        }
    }

    // Spawned → DataAvailable：需要有 PawnData + Controller
    if (CurrentState == Nyota::InitState_Spawned && DesiredState == Nyota::InitState_DataAvailable)
    {
        if (!PawnData)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("[InitTrace] PawnExtComp %s: CanChangeInitState Spawned→DataAvailable: FAIL - no PawnData"),
                *GetNameSafe(Pawn)
            );
            return false;
        }

        const bool bHasAuthority = Pawn->HasAuthority();
        const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

        if (bHasAuthority || bIsLocallyControlled)
        {
            if (!GetController<AController>())
            {
                UE_LOG(
                    LogTemp,
                    Warning,
                    TEXT("[InitTrace] PawnExtComp %s: CanChangeInitState Spawned→DataAvailable: FAIL - no Controller"),
                    *GetNameSafe(Pawn)
                );
                return false;
            }
        }

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("[InitTrace] PawnExtComp %s: CanChangeInitState Spawned→DataAvailable: PASS"),
            *GetNameSafe(Pawn)
        );
        return true;
    }

    // DataAvailable → DataInitialized：各 Feature 的初始化已通过 OnActorInitStateChanged 保证时序，
    // HeroComponent 的 InitializePlayerInput / InitializeAbilitySystem 在自身的 HandleChangeInitState 中执行。
    // PawnExtComp 的 HandleChangeInitState(DataInitialized) 为空，此处直接放行。
    if (CurrentState == Nyota::InitState_DataAvailable && DesiredState == Nyota::InitState_DataInitialized)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("[InitTrace] PawnExtComp %s: CanChangeInitState DataAvailable→DataInitialized: PASS (auto)"),
            *GetNameSafe(Pawn)
        );
        return true;
    }

    // DataInitialized → GameplayReady：自动放行
    if (CurrentState == Nyota::InitState_DataInitialized && DesiredState == Nyota::InitState_GameplayReady)
    {
        return true;
    }

    return false;
}

void UNyotaPawnExtensionComponent::HandleChangeInitState(
    UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
)
{
    if (DesiredState == Nyota::InitState_DataInitialized)
    {
        // 到达 DataInitialized 时，可以做一些集中初始化工作
        // （大部分实际初始化由各个 Feature 自己在其 HandleChangeInitState 里做）
    }
}

void UNyotaPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams &Params)
{
    // 任何其他 Feature 到达 DataAvailable → 检查自己能不能推进到 DataInitialized
    if (Params.FeatureName != NAME_ActorFeatureName)
    {
        if (Params.FeatureState == Nyota::InitState_DataAvailable)
        {
            CheckDefaultInitialization();
        }
    }
}

void UNyotaPawnExtensionComponent::CheckDefaultInitialization()
{
    // 先递归推进所有依赖的子 Feature
    CheckDefaultInitializationForImplementers(); // 引擎函数，递归推进子组件

    static const TArray<FGameplayTag> StateChain = {
        Nyota::InitState_Spawned,
        Nyota::InitState_DataAvailable,
        Nyota::InitState_DataInitialized,
        Nyota::InitState_GameplayReady
    };

    // 这将尝试从 InitState_Spawned(仅在 BeginPlay 中设置) 开始，经过数据初始化阶段，直到它准备好
    // InitState_GameplayReady
    ContinueInitStateChain(StateChain);
}

void UNyotaPawnExtensionComponent::SetPawnData(const UNyotaPawnData *InPawnData)
{
    check(InPawnData);

    APawn *Pawn = GetPawnChecked<APawn>();

    if (Pawn->GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    if (PawnData)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."),
            *GetNameSafe(InPawnData),
            *GetNameSafe(Pawn),
            *GetNameSafe(PawnData)
        );

        return;
    }

    // PawnData 就绪了 → 尝试推进！
    PawnData = InPawnData;

    Pawn->ForceNetUpdate();

    CheckDefaultInitialization();
}

void UNyotaPawnExtensionComponent::OnRegister()
{
    Super::OnRegister();

    const APawn *Pawn = GetPawn<APawn>();
    ensureAlwaysMsgf(
        Pawn != nullptr,
        TEXT("NyotaPawnExtensionComponent on [%s] can only be added to Pawn actors."),
        *GetNameSafe(GetOwner())
    );

    TArray<UActorComponent *> PawnExtensionComponents;
    Pawn->GetComponents(StaticClass(), PawnExtensionComponents);
    ensureAlwaysMsgf(
        PawnExtensionComponents.Num() == 1,
        TEXT("Only one NyotaPawnExtensionComponent should exist on [%s]."),
        *GetNameSafe(GetOwner())
    );

    // 把自己注册到 ComponentManager
    RegisterInitStateFeature();
    bHasRegisteredInitState = true;
}

void UNyotaPawnExtensionComponent::BeginPlay()
{
    Super::BeginPlay();

    // 监听所有其他 Feature 的状态变化（NAME_None = 监听全部）
    BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

    // Controller 绑定时重新触发 InitState 检查（Controller 可能晚于 BeginPlay 才绑定）
    if (APawn *Pawn = GetPawn<APawn>())
    {
        Pawn->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnPawnControllerChanged);
    }

    // 推进到 Spawned，然后尝试继续推进
    ensure(TryToChangeInitState(Nyota::InitState_Spawned));

    CheckDefaultInitialization();
}

void UNyotaPawnExtensionComponent::OnPawnControllerChanged(
    APawn *Pawn, AController *OldController, AController *NewController
)
{
    if (NewController != nullptr)
    {
        CheckDefaultInitialization();
    }
}

void UNyotaPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UninitializeAbilitySystem();

    if (bHasRegisteredInitState)
    {
        UnregisterInitStateFeature();
        bHasRegisteredInitState = false;
    }

    Super::EndPlay(EndPlayReason);
}

void UNyotaPawnExtensionComponent::OnRep_PawnData()
{
    // 客户端收到复制 → 尝试推进！
    CheckDefaultInitialization();
}

void UNyotaPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UNyotaPawnExtensionComponent, PawnData);
}
