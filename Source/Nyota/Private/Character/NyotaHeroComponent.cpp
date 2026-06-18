// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NyotaHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "InputMappingContext.h"
#include "Character/NyotaPawnData.h"
#include "Character/NyotaPawnExtensionComponent.h"
#include "GA/CustomAbilitySystemComponent.h"
#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"
#include "UserSettings/EnhancedInputUserSettings.h"

UNyotaHeroComponent::UNyotaHeroComponent(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
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
            }
        }
    }
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
