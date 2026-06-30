// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_Grab.generated.h"

class ABaseEnemyWildBoar;
/**
 *
 */
UCLASS()
class NYOTA_API UGA_Grab : public UNyotaGameplayAbility
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
    void OnGrabEnd(FGameplayEventData EventData);

    UFUNCTION()
    void OnGrabTimeout();

    UFUNCTION()
    void OnStartGrabTrace(FGameplayEventData EventData);

    UFUNCTION()
    void OnStopGrabTrace(FGameplayEventData EventData);

    UFUNCTION()
    void PerformGrabTrace();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> GrabMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float HitBoxRadius;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float ThrownForce;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float GrabTimerRate = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float GrabTraceRadius = 15.f;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float GrabTraceRate = 0.01;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    FName LeftHandSocketName;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    FName RightHandSocketName;

    FTimerHandle TimerHandle;

    FVector PrevLeftHandLocation;

    FVector PrevRightHandLocation;

    FTimerHandle GrabTraceTimerHandle;
};
