// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/GA_BaseHitReact.h"

void UGA_BaseHitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{
}

void UGA_BaseHitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UGA_BaseHitReact::NiaAttach(UNiagaraSystem *NiagaraSystem, FVector HitLocation, FRotator HitRotation)
{
}

void UGA_BaseHitReact::AttackAbout(ABaseCharacter *BaseCharacter, float AttackTime, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect, TSubclassOf<UGameplayEffect> HitEffectClass)
{
}

void UGA_BaseHitReact::Attack(ABaseCharacter *BaseCharacter, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect, TSubclassOf<UGameplayEffect> HitEffectClass)
{
}
