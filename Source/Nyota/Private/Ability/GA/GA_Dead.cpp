// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_Dead.h"
#include "Character/NyotaCharacters.h"
#include "Ability/NyotaAttributeSet.h"
bool UGA_Dead::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ANyotaCharacters* character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor.Get());

	float Health = character->GetAbilitySystemComponent()->GetNumericAttribute(UNyotaAttributeSet::GetHealthAttribute());
	float MaxHealth = character->GetAbilitySystemComponent()->GetNumericAttribute(UNyotaAttributeSet::GetMaxHealthAttribute());

	return Health == 0;
}

void UGA_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UGA_Dead::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


