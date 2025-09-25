// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystemComponent.h"
#include "Ability/GA/GA_Base.h"
#include "NyotaCharacterAbilityBaseConfig.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FNyota_AbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = InputTag))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGA_Base> AbilityToGrant;

};


UCLASS()
class NYOTA_API UNyotaCharacterAbilityBaseConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	//等待GA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> NyotaWaitingAbilities;

	//被动GA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> NyotaPassiveAbilities;

	//主动GA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<FNyota_AbilitySet> NyotaActiveAbilities;

	//被动GE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> NyotaPassiveEffect;




	UFUNCTION()
	void GiveAbilityToComponent(UAbilitySystemComponent* inComponent, int32 InLevel = 0);

};
