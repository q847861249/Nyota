// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

UAbilitySystemComponent *ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::GiveDefaultAbility()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility> &Ga : GAClass)
	{
		if (!Ga)
		{
			continue;
		}

		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ga.GetDefaultObject(), 1, 0));
	}
}
