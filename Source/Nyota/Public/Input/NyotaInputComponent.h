// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "NyotaInputConfig.h"
#include "NyotaInputComponent.generated.h"

struct FGameplayTag;
class UEnhancedInputLocalPlayerSubsystem;
class UNyotaInputConfig;

UCLASS(Config = Input)
class NYOTA_API UNyotaInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()

public:
    UNyotaInputComponent(const FObjectInitializer& ObjectInitializer);

    void AddInputMappings(
        const UNyotaInputConfig *InputConfig, UEnhancedInputLocalPlayerSubsystem *InputSubsystem
    ) const;

    void RemoveInputMappings(
        const UNyotaInputConfig *InputConfig, UEnhancedInputLocalPlayerSubsystem *InputSubsystem
    ) const;

    template <class UserClass, typename FuncType>
    void BindNativeAction(
        const UNyotaInputConfig *InputConfig, const FGameplayTag &InputTag, ETriggerEvent TriggerEvent,
        UserClass *Object, FuncType Func, bool bLogIfNotFound
    );

    template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
    void BindAbilityActions(
        const UNyotaInputConfig *InputConfig, UserClass *Object, PressedFuncType PressedFunc,
        ReleasedFuncType ReleasedFunc, TArray<uint32> &BindHandles
    );

    void RemoveBind(TArray<uint32> &BindHandles);
};

template <class UserClass, typename FuncType>
void UNyotaInputComponent::BindNativeAction(
    const UNyotaInputConfig *InputConfig, const FGameplayTag &InputTag, ETriggerEvent TriggerEvent, UserClass *Object,
    FuncType Func, bool bLogIfNotFound
)
{
    check(InputConfig);

    if (const UInputAction *IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
    {
        BindAction(IA, TriggerEvent, Object, Func);
    }
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UNyotaInputComponent::BindAbilityActions(
    const UNyotaInputConfig *InputConfig, UserClass *Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
    TArray<uint32> &BindHandles
)
{
    check(InputConfig);

    for (const FNyotaInputAction &Action : InputConfig->AbilityInputActions)
    {
        if (Action.InputAction && Action.InputTag.IsValid())
        {
            if (PressedFunc)
            {
                BindHandles.Add(BindActionValueLambda(
                    Action.InputAction,
                    ETriggerEvent::Triggered,
                    [Object, PressedFunc, InputTag = Action.InputTag](const FInputActionValue &)
                    {
                        (Object->*PressedFunc)(InputTag);
                    }
                ).GetHandle());
            }

            if (ReleasedFunc)
            {
                BindHandles.Add(BindActionValueLambda(
                    Action.InputAction,
                    ETriggerEvent::Completed,
                    [Object, ReleasedFunc, InputTag = Action.InputTag](const FInputActionValue &)
                    {
                        (Object->*ReleasedFunc)(InputTag);
                    }
                ).GetHandle());
            }
        }
    }
}
