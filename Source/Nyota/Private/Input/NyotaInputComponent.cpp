// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/NyotaInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NyotaInputComponent)

UNyotaInputComponent::UNyotaInputComponent(const FObjectInitializer &ObjectInitializer)
{
}

void UNyotaInputComponent::AddInputMappings(
    const UNyotaInputConfig *InputConfig, UEnhancedInputLocalPlayerSubsystem *InputSubsystem
) const
{
    check(InputConfig);
    check(InputSubsystem);

    // Here you can handle any custom logic to add something from your input config if required
}

void UNyotaInputComponent::RemoveInputMappings(
    const UNyotaInputConfig *InputConfig, UEnhancedInputLocalPlayerSubsystem *InputSubsystem
) const
{
    check(InputConfig);
    check(InputSubsystem);

    // Here you can handle any custom logic to remove input mappings that you may have added above
}

void UNyotaInputComponent::RemoveBind(TArray<uint32> &BindHandles)
{
    for (uint32 Handle : BindHandles)
    {
        RemoveBindingByHandle(Handle);
    }

    BindHandles.Reset();
}
