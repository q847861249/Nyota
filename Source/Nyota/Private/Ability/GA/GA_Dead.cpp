// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_Dead.h"
#include "Character/NyotaCharacters.h"
#include "Ability/NyotaAttributeSet.h"
#include "Debug/Debug.h"
#include "Input/NyotaGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

bool UGA_Dead::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ANyotaCharacters* character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor.Get());

	// check character health on attribute 
	return character->GetAbilitySystemComponent()->GetNumericAttribute(UNyotaAttributeSet::GetHealthAttribute()) == 0;
}

void UGA_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ANyotaCharacters* character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor.Get());


	//FGameplayEventData Payload;
	//Payload.EventTag = FNyotaGameplayTags::Get().State_RagDoll;
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(character, FNyotaGameplayTags::Get().State_RagDoll, Payload);

	character->EnableRagDoll();

	character->SetLifeSpan(2.0f);
	
	K2_EndAbility();

}

void UGA_Dead::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


