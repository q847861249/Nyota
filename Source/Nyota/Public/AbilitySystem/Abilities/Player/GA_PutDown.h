// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"

#include "GA_PutDown.generated.h"

/** Gently releases the player's currently grabbed target. */
UCLASS()
class NYOTA_API UGA_PutDown : public UNyotaGameplayAbility
{
    GENERATED_BODY()

#if WITH_DEV_AUTOMATION_TESTS
    friend class FPutDownAbilityContractTest;
#endif

public:
    UGA_PutDown(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float ThrownForce;
};
