// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "KillAbility.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UKillAbility : public UBaseAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ApplyKill(TSubclassOf<UGameplayEffect> KillEffClass,float KillReward);
};
