// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "HitReact.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UHitReact : public UBaseAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(TSubclassOf<UGameplayEffect> GEClass, float Damage);
};
