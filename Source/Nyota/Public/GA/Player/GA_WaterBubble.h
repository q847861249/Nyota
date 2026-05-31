// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseSkill.h"
#include "GA_WaterBubble.generated.h"

class ABaseProjectile;
/**
 *
 */
UCLASS()
class NYOTA_API UGA_WaterBubble : public UGA_BaseSkill
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

protected:
    UFUNCTION()
    void OnStartMontageCompleted();

    UFUNCTION()
    void OnEndMontageCompleted();
    
    UFUNCTION()
    void SpawnWaterBubble();

    UFUNCTION()
    void OnStartMontageInterrupted();

    UFUNCTION()
    void OnWaterBubbleEnd(FGameplayEventData EventData);

    UFUNCTION()
    void OnWaterBubbleHit(const FHitResult &HitResult);

    UPROPERTY(BlueprintReadOnly, Category = "Nyota")
    bool bIsStopping;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> StartMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> LoopMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> EndMontage;

    UPROPERTY(BlueprintReadOnly, Category = "Nyota")
    FTimerHandle TimerHandle;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    FName FireSockName;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<ABaseProjectile> WaterBubbleClass;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<UGameplayEffect> HitEffectClass;
};
