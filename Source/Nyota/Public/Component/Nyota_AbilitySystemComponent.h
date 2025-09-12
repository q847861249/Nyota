// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Nyota_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UNyota_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	

public:

	void OnAbilityPressed(const FGameplayTag& GameplayTag);

	void OnAbilityReleased(const FGameplayTag& GameplayTag);


};
