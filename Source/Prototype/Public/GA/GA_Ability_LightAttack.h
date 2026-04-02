// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseHitReact.h"
#include "GA_Ability_LightAttack.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API UGA_Ability_LightAttack : public UGA_BaseHitReact
{
    GENERATED_BODY()

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
    ) override;

protected:
    UPROPERTY(EditAnywhere)
    UAnimMontage *AttackMontage;

    UPROPERTY(EditAnywhere)
    UAnimMontage *TargetHitReactMontage;

    UPROPERTY(EditAnywhere)
    float AttackTime;

    UPROPERTY(EditAnywhere)
    UNiagaraSystem *CastNiagaraEffect;

    UPROPERTY(EditAnywhere, Category = "Gameplay Effect")
    TSubclassOf<UGameplayEffect> HitEffectClass;
};
