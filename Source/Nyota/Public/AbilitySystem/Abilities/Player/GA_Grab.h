// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_Grab.generated.h"

class ABaseEnemyWildBoar;
class UAbilitySystemComponent;
/**
 *
 */
UCLASS()
class NYOTA_API UGA_Grab : public UNyotaGameplayAbility
{
    GENERATED_BODY()

#if WITH_DEV_AUTOMATION_TESTS
    friend class FPutDownWindowLifecycleTest;
#endif

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
    ) override;

    UFUNCTION()
    void OnMontageCompleted();

    UFUNCTION()
    void OnGrabEnd(FGameplayEventData EventData);

    UFUNCTION()
    void OnPutDownTimeout();

    UFUNCTION()
    void OnPutDownEnd(FGameplayEventData EventData);

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

private:
    void AddPutDownWindowTag(UAbilitySystemComponent *AbilitySystemComponent);

    void RemovePutDownWindowTag(UAbilitySystemComponent *AbilitySystemComponent);

    void ReleaseGrabbedTargetAsFailSafe();

    TWeakObjectPtr<UAbilitySystemComponent> PutDownWindowAbilitySystemComponent;

    bool bOwnsPutDownWindowTag = false;
};
