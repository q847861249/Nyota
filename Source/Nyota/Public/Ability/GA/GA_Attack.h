// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GA/GA_Base.h"
#include "DataAsset/DamgeConfig.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA_Attack.generated.h"


/**
 * 
 */
UCLASS()
class NYOTA_API UGA_Attack : public UGA_Base
{
	GENERATED_BODY()
	
public:

	UGA_Attack();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
	
	UFUNCTION()
	void shooting(FGameplayEventData Payload);

public:

	UPROPERTY(EditDefaultsOnly)
	UDamgeConfig* DamageConfig;

	// Montage Task For Attack
	UAbilityTask_PlayMontageAndWait* MontageTask;

	//Event Task for spawn projectile
	UAbilityTask_WaitGameplayEvent* EventTask;
};