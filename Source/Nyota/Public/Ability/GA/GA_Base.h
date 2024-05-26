// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Base.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UGA_Base : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
	UFUNCTION(BlueprintCallable,BlueprintPure) ANyotaCharacters* getAvatarNyotaActor();

protected:

	//** ��󴥷���effect **//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	//** ��ʼ������effect **//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyStart;

	//** ���Ҫ������effect **//
	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandle;

	


};
