// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/NyotaInputConfig.h"

UNyotaInputConfig::UNyotaInputConfig(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

const UInputAction *UNyotaInputConfig::FindNativeInputActionForTag(
    const FGameplayTag &InputTag, bool bLogNotFound
) const
{
    for (const FNyotaInputAction &Action : NativeInputActions)
    {
        if (Action.InputAction && Action.InputTag == InputTag)
        {
            return Action.InputAction;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
            *InputTag.ToString(),
            *GetNameSafe(this)
        );
    }

    return nullptr;
}

const UInputAction *UNyotaInputConfig::FindAbilityInputActionForTag(
    const FGameplayTag &InputTag, bool bLogNotFound
) const
{
    for (const FNyotaInputAction &Action : AbilityInputActions)
    {
        if (Action.InputAction && Action.InputTag == InputTag)
        {
            return Action.InputAction;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."),
            *InputTag.ToString(),
            *GetNameSafe(this)
        );
    }

    return nullptr;
}