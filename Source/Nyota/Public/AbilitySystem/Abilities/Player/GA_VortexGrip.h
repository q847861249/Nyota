// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_VortexGrip.generated.h"

class ABaseCharacter;
/**
 *
 */
UCLASS()
class NYOTA_API UGA_VortexGrip : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    UFUNCTION()
    void StartVortexGrip();

    UFUNCTION()
    void ApplyDamage(FGameplayEventData EventData);

    UFUNCTION()
    void OnAbilityEnd();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> VortexGripMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<UGameplayEffect> VortexGripEffect;
};
