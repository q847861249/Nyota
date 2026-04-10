// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "PickUpAbility.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UPickUpAbility : public UBaseAbility
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> PickUpCoinEffectClass;
public:
	UFUNCTION(BlueprintCallable)
	void PickUpCoin(AActor* PickUpItem);
};
