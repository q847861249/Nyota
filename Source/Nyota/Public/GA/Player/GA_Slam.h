// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseSkill.h"
#include "GA_Slam.generated.h"

class ABaseCharacter;
/**
 *
 */
UCLASS()
class NYOTA_API UGA_Slam : public UGA_BaseSkill
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    UFUNCTION()
    void StartSlam();

    UFUNCTION()
    void OnApplySlamDamage(FGameplayEventData EventData);

    UFUNCTION()
    void OnSlamEnd();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> SlamMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<UGameplayEffect> SlamEffect;
};
