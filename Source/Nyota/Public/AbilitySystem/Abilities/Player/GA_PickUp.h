// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_PickUp.generated.h"

/**
 *
 */
class UAnimMontage;
class ALoot;
UCLASS()
class NYOTA_API UGA_PickUp : public UNyotaGameplayAbility
{
    GENERATED_BODY()
private:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UGameplayEffect> PickUpEffectClass;   
    UPROPERTY()
    TObjectPtr<ALoot> CurrentLoot;
protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
    ) override;

    /**玩家角色的捕获体触发碰撞后执行 */
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult
    );
    void PickUpLoot(AActor* Actor);
    void Channeling();
    void Orientation();
    UFUNCTION()
    void RestorePlayerInput();
    UFUNCTION()
    void CleanPickUpUI();
    UFUNCTION()
    void OnPickUpComplete();
public:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAnimMontage> PickUpMontage;
  
};
