// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/GA/GA_Base.h"
#include "Weapon/NyotaWeapon.h"
#include "GA_SpawnWeapon.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UGA_SpawnWeapon : public UGA_Base
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponConfig")
	FName WeaponSpawnSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponConfig")
	TSubclassOf<ANyotaWeapon> WeaponBlueprintClass;


public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


};
