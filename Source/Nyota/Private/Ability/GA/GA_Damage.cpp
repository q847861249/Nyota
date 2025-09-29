// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability/GA/GA_Damage.h"
#include "Character/NyotaCharacters.h"


void UGA_Damage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// effect loop

	if (ANyotaCharacters* character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor))
	{

		FGameplayEffectContextHandle EffectContext = character->AbilitySystem->MakeEffectContext();

		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> effect : DamageEffect)
		{
			character->ApplyGameplayEffectToself(effect, EffectContext);
		}
	}
}
