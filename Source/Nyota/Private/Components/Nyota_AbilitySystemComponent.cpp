// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Nyota_AbilitySystemComponent.h"
#include "Tags/Nyota_Tag.h"

void UNyota_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) 
{
	Super::OnGiveAbility(AbilitySpec);

	ActivateAbilityWithInitTag(AbilitySpec);
}

void UNyota_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
    // Add Lock before looping AbilityList
    //Prevent the list from being modified during use
    FScopedAbilityListLock AbilityListLock(*this);

    for (const FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
    {
        ActivateAbilityWithInitTag(AbilitySpec);
    }
}

void UNyota_AbilitySystemComponent::ActivateAbilityWithInitTag(const FGameplayAbilitySpec& AbilitySpec)
{
    if(!IsValid(AbilitySpec.Ability)) return;

    if (AbilitySpec.Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::ServerInitiated 
        && !IsOwnerActorAuthoritative())
    {
        return;
    }

	if (AbilitySpec.Ability->AbilityTags.HasTagExact(Nyota::Ability::Init))
	{
		TryActivateAbility(AbilitySpec.Handle);
        return;
	}
    
}
