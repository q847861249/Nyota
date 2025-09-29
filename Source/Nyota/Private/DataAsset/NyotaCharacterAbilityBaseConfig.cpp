// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/NyotaCharacterAbilityBaseConfig.h"
#include "GameplayAbilitySpec.h"
#include <Debug/Debug.h>

void UNyotaCharacterAbilityBaseConfig::GiveAbilityToComponent(UAbilitySystemComponent* inComponent, int32 InLevel)
{
	check(inComponent);

	if (!NyotaPassiveAbilities.IsEmpty())
	{
		for (TSubclassOf<UGameplayAbility>Ability : NyotaPassiveAbilities)
		{
			if (Ability)
			{
				FGameplayAbilitySpec ABilitySpec(Ability);
				ABilitySpec.SourceObject = inComponent->GetAvatarActor();
				ABilitySpec.Level = InLevel;

				inComponent->GiveAbility(ABilitySpec);
			}

		}
	}

	if (!NyotaActiveAbilities.IsEmpty())
	{
		for (FNyota_AbilitySet& AbilitySet : NyotaActiveAbilities)
		{
			FGameplayAbilitySpec ABilitySpec(AbilitySet.AbilityToGrant);
			ABilitySpec.SourceObject = inComponent->GetAvatarActor();
			ABilitySpec.Level = InLevel;
			ABilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

			inComponent->GiveAbility(ABilitySpec).IsValid();

		}
	}
}
