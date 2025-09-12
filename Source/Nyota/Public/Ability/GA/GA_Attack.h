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



private:

	//Light attack monatge store list
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TMap<int32, UAnimMontage*> LightAttackMontage;

	//Melee Montage count index 
	int32 CurrrentComboCount = 1;


protected:
	UPROPERTY()
	TObjectPtr<UNyotaComponent> NyotaComponent;

	/** Change to play the montage faster or slower */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	float Rate = 1.f;

	/** Any gameplay events matching this tag will activate the OnEventReceived callback and apply the gameplay effect containers for this ability */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	FGameplayTagContainer WaitForEventTag;


};
