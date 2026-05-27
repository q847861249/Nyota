// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BaseSkill.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UGA_BaseSkill : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_BaseSkill();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Debug")
    bool bDrawDebugs = false;

protected:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyota | OverlapTest")
    FVector GetAbilityDetectionDirection() const;

    virtual FVector GetAbilityDetectionDirection_Implementation() const;
};
