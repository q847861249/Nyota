// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_OnHit.h"
#include "AbilitySystemComponent.h"
#include "Ability/NyotaAttributeSet.h"
#include "Component/NyotaComponent.h"
#include "Input/NyotaGameplayTags.h"
#include "GameplayEffectTypes.h"

#include "Character/NyotaCharacters.h"

bool UGA_OnHit::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ANyotaCharacters* Character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor.Get());
	if (!Character) return false;
	if (!Character->NyotaComponent) return false;

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_OnHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//受击动画
	if (UAnimMontage* OnHit_AnimMontage = getAvatarNyotaActor()->NyotaComponent->CharacterConfig->CharacterCombatConfig->OnHit.AnimMontage) {

		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, OnHit_AnimMontage);
		MontageTask->ReadyForActivation();

		MontageTask->OnBlendOut.AddDynamic(this, &UGA_OnHit::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_OnHit::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_OnHit::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_OnHit::K2_EndAbility);
	}
	else {
		K2_EndAbility();
	}
}

void UGA_OnHit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	OnHitEffect(ActorInfo);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_OnHit::OnHitEffect(const FGameplayAbilityActorInfo* ActorInfo)
{
	ANyotaCharacters* character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor.Get());

	float BasicDamage = character->NyotaComponent->CurrentActivateProjectileInfo.ProjectileProperty.BasicDamage;

	float CurrentHealth = character->GetAbilitySystemComponent()->GetNumericAttributeBase(UNyotaAttributeSet::GetHealthAttribute());
	float CurrentMaxHealth = character->GetAbilitySystemComponent()->GetNumericAttributeBase(UNyotaAttributeSet::GetMaxHealthAttribute());

	//calculate basic damage
	if (CurrentHealth + BasicDamage >= CurrentMaxHealth) {}
	else if (CurrentHealth <= 0) {}
	else if ((CurrentHealth + BasicDamage <= 0)) character->GetAbilitySystemComponent()->SetNumericAttributeBase(UNyotaAttributeSet::GetHealthAttribute(), 0);
	else character->GetAbilitySystemComponent()->SetNumericAttributeBase(UNyotaAttributeSet::GetHealthAttribute(), CurrentHealth + BasicDamage);

	//理论上会损失性能但是问题应该不大
	character->TryActiveAbilityByTag(FNyotaGameplayTags::Get().State_Dead);

}
