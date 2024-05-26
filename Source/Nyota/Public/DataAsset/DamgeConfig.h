// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "DamgeConfig.generated.h"


UENUM(BlueprintType)
enum EDizzy
{
	Dizzy_Range  UMETA(DisplayName = "Range"),

	Dizzy_Single  UMETA(DisplayName = "Single"),

};


USTRUCT(Blueprintable)
struct FCombcfg
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EDizzy> DizzyType;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* FX;

};

UCLASS()
class NYOTA_API UDamgeConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	FCombcfg CombConfig;

};
