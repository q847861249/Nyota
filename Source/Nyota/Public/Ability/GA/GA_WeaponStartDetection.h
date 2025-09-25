// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GA/GA_Base.h"
#include "GA_WeaponStartDetection.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UGA_WeaponStartDetection : public UGA_Base
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
};
