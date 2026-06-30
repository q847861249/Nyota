// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_WaterBall.generated.h"

class ABaseProjectile;
/**
 *
 */
UCLASS()
class NYOTA_API UGA_WaterBall : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    UFUNCTION()
    void OnMontageCompleted();

    UFUNCTION()
    void OnWaitGameplayEvent(FGameplayEventData EventData);

    UFUNCTION()
    void SpawnWaterBall();

protected:
    UFUNCTION()
    void OnWaterBallHit(const FHitResult &HitResult);

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> ShootWaterBallMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    FName FireSockName;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<ABaseProjectile> WaterBallClass;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<UGameplayEffect> HitEffectClass;
};
