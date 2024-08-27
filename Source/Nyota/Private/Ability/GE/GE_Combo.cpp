// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GE/GE_Combo.h"
#include "Input/NyotaGameplayTags.h"
#include "Debug/Debug.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"



void UGE_Combo::PostInitProperties()
{
	Super::PostInitProperties();



	DurationPolicy = EGameplayEffectDurationType::Infinite;

}

