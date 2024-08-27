// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GA/GA_Base.h"
#include "DataAsset/DamgeConfig.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA_Attack.generated.h"



class UNyotaComponent;
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
	
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr);



	UFUNCTION()
	void shooting(FGameplayEventData Payload);



public:


	// Montage Task For Attack
	UAbilityTask_PlayMontageAndWait* MontageTask;

	//Event Task for spawn projectile
	UAbilityTask_WaitGameplayEvent* EventTask;


protected:
	UPROPERTY()
	TObjectPtr<UNyotaComponent> NyotaComponent;

	/** List of animation montages you want to cycle through when activating this ability */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TArray<TObjectPtr<UAnimMontage>> Montages;

	/** Change to play the montage faster or slower */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	float Rate = 1.f;

	/** Any gameplay events matching this tag will activate the OnEventReceived callback and apply the gameplay effect containers for this ability */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	FGameplayTagContainer WaitForEventTag;

	UFUNCTION(BlueprintPure, Category = "GAS Companion|Ability|Melee")
	UAnimMontage* GetNextComboMontage();


	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

};
