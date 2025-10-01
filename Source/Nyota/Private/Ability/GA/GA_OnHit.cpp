// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_OnHit.h"
#include "AbilitySystemComponent.h"
#include "Ability/NyotaAttributeSet.h"
#include "Component/NyotaComponent.h"
#include "Input/NyotaGameplayTags.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Ability/GA/GA_Dead.h"

#include "Character/NyotaCharacters.h"

bool UGA_OnHit::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ANyotaCharacters* Character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor.Get());
	if (!Character) return false;

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_OnHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);



	if (!OnHitMontage) return;


	if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, OnHitMontage))
	{

		Task->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		Task->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		Task->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		Task->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);

		Task->ReadyForActivation();
	}

}

void UGA_OnHit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	//remove 'on hit' tag after ability execution 
	ActorInfo->AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(AbilityTags);

	// active GA_Dead, and check the character health on CanActivateAbility 
	ActorInfo->AbilitySystemComponent->TryActivateAbilityByClass(DeadGA);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

