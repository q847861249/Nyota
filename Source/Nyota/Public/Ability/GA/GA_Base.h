// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"

#include "GA_Base.generated.h"




USTRUCT(BlueprintType)
struct FGSCGameplayEffectContainer
{
	GENERATED_BODY()

public:
	FGSCGameplayEffectContainer() {}

	/** Sets the way that targeting happens */


	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	bool bUseSetByCallerMagnitude = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayTag SetByCallerDataTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	float SetByCallerMagnitude = 1.0f;
};


USTRUCT(BlueprintType)
struct NYOTA_API FGSCGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	FGSCGameplayEffectContainerSpec() {}

	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

};




UCLASS()
class NYOTA_API UGA_Base : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
	
	UFUNCTION(BlueprintCallable,BlueprintPure) 
	ANyotaCharacters* getAvatarNyotaActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FGSCGameplayEffectContainer> EffectContainerMap;

protected:

	//** 最后触发的effect **//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	//** 开始触发的effect **//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyStart;


	//** 最后要结束的effect **//
	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandle;

	
	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|Ability")
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FGSCGameplayEffectContainerSpec& ContainerSpec);

	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|Ability", meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);


	FGSCGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel);

	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|Ability", meta = (AutoCreateRefTerm = "EventData"))
	virtual FGSCGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FGSCGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);
};
