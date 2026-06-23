// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NyotaHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"
#include "Character/NyotaPawnData.h"
#include "Character/NyotaPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GA/CustomAbilitySystemComponent.h"
#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"
#include "GameplayTags/GameTags.h"
#include "Input/NyotaInputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"

namespace NyotaHero
{
static constexpr float LookYawRate = 300.0f;
static constexpr float LookPitchRate = 165.0f;
}; // namespace NyotaHero

const FName UNyotaHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UNyotaHeroComponent::NAME_ActorFeatureName("Hero");

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
