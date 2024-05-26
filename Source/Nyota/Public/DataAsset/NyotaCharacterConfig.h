// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AbilitySystemComponent.h"
#include "DataAsset/NyotaCharacterAbilityBaseConfig.h"
#include "DataAsset/NyotaCharacterCombatConfig.h"
#include "Weapon/NyotaWeapon.h"

#include "NyotaCharacterConfig.generated.h"


USTRUCT(Blueprintable)
struct FWeapon
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ANyotaWeapon> NyotaWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketName;

};



UCLASS()
class NYOTA_API UNyotaCharacterConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	UNyotaCharacterAbilityBaseConfig* CharacterAbilityConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UNyotaCharacterCombatConfig* CharacterCombatConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TArray<FWeapon> NyotaWeapon;

};
