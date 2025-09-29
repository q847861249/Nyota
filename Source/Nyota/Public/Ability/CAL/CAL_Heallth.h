// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "CAL_Heallth.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UCAL_Heallth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UCAL_Heallth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition HealthDef;
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	FGameplayEffectAttributeCaptureDefinition MinHealthDef;
};
