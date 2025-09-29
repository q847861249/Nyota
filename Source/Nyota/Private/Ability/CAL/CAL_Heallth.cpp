// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAL/CAL_Heallth.h"
#include "Ability/NyotaAttributeSet.h"

UCAL_Heallth::UCAL_Heallth()
{
	HealthDef.AttributeToCapture = UNyotaAttributeSet::GetHealthAttribute();
	HealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	HealthDef.bSnapshot = false;

	MaxHealthDef.AttributeToCapture = UNyotaAttributeSet::GetMaxHealthAttribute();
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxHealthDef.bSnapshot = false;

	MinHealthDef.AttributeToCapture = UNyotaAttributeSet::GetMinHealthAttribute();
	MinHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MinHealthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(HealthDef);
	RelevantAttributesToCapture.Add(MaxHealthDef);
	RelevantAttributesToCapture.Add(MinHealthDef);
}

float UCAL_Heallth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalutionParameters;
	EvalutionParameters.SourceTags = SourceTags;
	EvalutionParameters.TargetTags = TargetTags;

	float Health = 0.0f;
	GetCapturedAttributeMagnitude(HealthDef, Spec, EvalutionParameters, Health);

	float MaxHealth = 0.0f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvalutionParameters, MaxHealth);

	float MinHealth = 0.0f;
	GetCapturedAttributeMagnitude(MinHealthDef, Spec, EvalutionParameters, MinHealth);

	if (MinHealth < Health && Health <= MaxHealth)
	{
		return 1;
	}

	return 0;
}


