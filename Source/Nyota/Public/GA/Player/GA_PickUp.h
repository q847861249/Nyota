// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseSkill.h"
#include "GA_PickUp.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UGA_PickUp : public UGA_BaseSkill
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> PickUpCoinEffectClass;
public:
	UFUNCTION(BlueprintCallable)
	void PickUpCoin(AActor* PickUpItem);
};
