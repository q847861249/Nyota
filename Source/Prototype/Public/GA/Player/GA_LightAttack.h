// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseSkill.h"
#include "GA_LightAttack.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API UGA_LightAttack : public UGA_BaseSkill
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    UFUNCTION(BlueprintCallable, Category = "Nyota | OverlapTest")
    TArray<AActor *> HitBoxOverlapTest();

    UFUNCTION(BlueprintCallable, Category = "Nyota | OverlapTest")
    void SendHitReactEventToActor(const TArray<AActor *> &HitActors);

    UFUNCTION(BlueprintCallable, Category = "Nyota | OverlapTest")
    void SetLookAtEnemyRotation(AActor *LookAtActor) const;

    UFUNCTION()
    void OnMontageCompleted();

    UFUNCTION()
    void OnLightAttack(FGameplayEventData EventData);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | OverlapTest")
    TObjectPtr<UAnimMontage> AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | OverlapTest")
    TSubclassOf<UGameplayEffect> DamageEffect;

private:
    void DrawDebugInformation(const TArray<FOverlapResult> &OverlapResults, const FVector &HitBoxLocation) const;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | OverlapTest")
    float HitBoxRadius = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | OverlapTest")
    float HitBoxForwardOffset = 0.f;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | OverlapTest")
    float HitBoxElevationOffset = 20.f;
};
