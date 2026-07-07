// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_PressureBlast.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UGA_PressureBlast : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    UFUNCTION()
    void Execute();

    UFUNCTION()
    void Terminate();

    UFUNCTION()
    void OnApplyDamage(FGameplayEventData Payload);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> PressureBlastMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<UGameplayEffect> PressureBlastEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float PressureBlastForce = 500.f;
};
