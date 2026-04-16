// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "DeathAbility.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UDeathAbility : public UBaseAbility
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan;
public:
	UFUNCTION(BlueprintCallable)
	void ApplyDeatn();
};
