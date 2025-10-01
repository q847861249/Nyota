// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GA/GA_Base.h"
#include "DataAsset/DamgeConfig.h"
#include "DataAsset/ProjectileConfig.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Ability/GA/GA_Dead.h"

#include "GA_OnHit.generated.h"



/**
 * 
 */
UCLASS()
class NYOTA_API UGA_OnHit : public UGA_Base
{
	GENERATED_BODY()
	

public:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);


	
public:

 	//Hit montage store list
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* OnHitMontage;

	//Dead GA Class here
	UPROPERTY(EditDefaultsOnly, Category = "GA_Dead")
	TSubclassOf<UGA_Dead> DeadGA;

};
