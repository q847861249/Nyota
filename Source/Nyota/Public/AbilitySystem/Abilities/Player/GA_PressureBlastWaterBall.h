// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_PressureBlastWaterBall.generated.h"

class ABaseProjectile;
/**
 *
 */
UCLASS()
class NYOTA_API UGA_PressureBlastWaterBall : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData *TriggerEventData
    ) override;

    UFUNCTION()
    void Execute();

    UFUNCTION()
    void Terminate();

    UFUNCTION()
    void OnShootWaterBall(FGameplayEventData EventData);

    UFUNCTION()
    void SpawnWaterBall();

    UFUNCTION()
    void OnApplyDamage(FGameplayEventData Payload);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> PressureBlastWaterBallMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    FName FireSocketName;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<ABaseProjectile> WaterBallClass;

    // 对被喷射的敌人结算的伤害效果
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<UGameplayEffect> PressureBlastEffect;

    // 将敌人喷射出去的力；同时也决定水球发射方向，使水球与敌人弧线保持一致
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota")
    float PressureBlastForce = 500.f;
};
