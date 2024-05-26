// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Particles/ParticleSystem.h"
#include "NyotaCharacterCombatConfig.generated.h"

USTRUCT(Blueprintable)
struct FAttack
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AnimMontage;

};

USTRUCT(Blueprintable)
struct FOnHit
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ParticleOnHit;

};

USTRUCT(Blueprintable)
struct FOnDead
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ParticleOnDead;

};


UCLASS()
class NYOTA_API UNyotaCharacterCombatConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAttack Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FOnHit OnHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FOnDead OnDead;

	
};
