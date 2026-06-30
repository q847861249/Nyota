// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Tasks/AbilityTask_WaitInputActionReleased.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

UAbilityTask_WaitInputActionReleased *UAbilityTask_WaitInputActionReleased::WaitInputActionReleased(
    UGameplayAbility *OwningAbility, UInputAction *InInputAction
)
{
    UAbilityTask_WaitInputActionReleased *Task = NewAbilityTask<UAbilityTask_WaitInputActionReleased>(OwningAbility);
    Task->InputAction = InInputAction;
    return Task;
}

void UAbilityTask_WaitInputActionReleased::Activate()
{
    Super::Activate();

    if (!InputAction)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: InputAction is null."), *GetNameSafe(this));
        EndTask();
        return;
    }

    APawn *Pawn = Cast<APawn>(GetAvatarActor());
    if (!IsValid(Pawn))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: Avatar is not a valid Pawn."), *GetNameSafe(this));
        EndTask();
        return;
    }

    // 仅在本地控制的客户端绑定输入，服务器和模拟端不需要
    if (!Pawn->IsLocallyControlled())
    {
        EndTask();
        return;
    }

    UEnhancedInputComponent *EnhancedInputComp = Cast<UEnhancedInputComponent>(Pawn->InputComponent);
    if (!EnhancedInputComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: Pawn InputComponent is not an EnhancedInputComponent."), *GetNameSafe(this));
        EndTask();
        return;
    }

    CachedInputComponent = EnhancedInputComp;

    FEnhancedInputActionEventBinding &Binding = CachedInputComponent->BindAction(
        InputAction,
        ETriggerEvent::Completed,
        this,
        GET_FUNCTION_NAME_CHECKED(UAbilityTask_WaitInputActionReleased, OnInputActionCompleted)
    );
    BindingHandle = Binding.GetHandle();

    StartTime = GetWorld()->GetTimeSeconds();
}

void UAbilityTask_WaitInputActionReleased::OnDestroy(bool bInOwnerFinished)
{
    if (IsValid(CachedInputComponent))
    {
        CachedInputComponent->RemoveBindingByHandle(BindingHandle);
    }

    Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitInputActionReleased::OnInputActionCompleted()
{
    const float TimeHeld = GetWorld()->GetTimeSeconds() - StartTime;
    OnRelease.Broadcast(TimeHeld);
    EndTask();
}