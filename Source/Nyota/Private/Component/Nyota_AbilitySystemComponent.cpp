// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Nyota_AbilitySystemComponent.h"
#include "Debug/Debug.h"

void UNyota_AbilitySystemComponent::OnAbilityPressed(const FGameplayTag& GameplayTag)
{

	if (!GameplayTag.IsValid()) return;



	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{

		if (AbilitySpec.DynamicAbilityTags.HasTagExact(GameplayTag)) 
		{

			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UNyota_AbilitySystemComponent::OnAbilityReleased(const FGameplayTag& GameplayTag)
{


}
