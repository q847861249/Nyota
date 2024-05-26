// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

#include "ProjectileConfig.generated.h"




UENUM(BlueprintType)
enum EDebuff
{
	None UMETA(DisplayName = "None"),

	Debuff_Dizzy  UMETA(DisplayName = "Dizzy"),

	Debuff_Burn  UMETA(DisplayName = "Burn"),

	Debuff_Poison UMETA(DisplayName = "Poison"),

	Debuff_Cold UMETA(DisplayName = "Cold")
};


class AProjectileBase;

USTRUCT(Blueprintable)
struct FProjectileProperty {

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BasicDamage;

};

USTRUCT(Blueprintable)
struct FProjectileVisual {

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ParticleOnSpawn;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SpawnSound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ParticleOnHit;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* HitSound;
};



USTRUCT(Blueprintable)
struct FProjectileCFG
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FProjectileVisual ProjectileVisual;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FProjectileProperty ProjectileProperty;
};

UCLASS()
class NYOTA_API UProjectileConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FProjectileCFG> ProjectileConFig;
	
};
