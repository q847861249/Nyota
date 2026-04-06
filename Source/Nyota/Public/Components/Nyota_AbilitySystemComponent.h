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
	// server side only, called when an ability is given to the ASC's owner.
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	// client side onlu, called when an ability is activated on the ASC's owner.
	virtual void OnRep_ActivateAbilities() override;
protected:
private:
	// Activate abilities which has "Init" tag
	void ActivateAbilityWithInitTag(const FGameplayAbilitySpec& AbilitySpec);
};
