// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_PickUp.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UGA_PickUp : public UNyotaGameplayAbility
{
    GENERATED_BODY()
private:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UGameplayEffect> PickUpEffectClass;   
protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    /**玩家角色的捕获体触发碰撞后执行 */
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult
    );
    void PickUpLoot(AActor* Actor);
    void Channeling();
public:
    UFUNCTION(BlueprintCallable)
    void PickUpCoin(AActor *PickUpItem);
  
};
