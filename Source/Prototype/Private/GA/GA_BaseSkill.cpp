// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/GA_BaseSkill.h"

UGA_BaseSkill::UGA_BaseSkill()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_BaseSkill::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (bDrawDebugs && IsValid(GEngine))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("%s Activated"), *GetName()));
    }
}