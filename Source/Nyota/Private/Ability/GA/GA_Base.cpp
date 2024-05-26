// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_Base.h"
#include "Character/NyotaCharacters.h"

bool UGA_Base::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;

	const ANyotaCharacters* Character = CastChecked<ANyotaCharacters>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	return true;
}

void UGA_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

		for (auto effect : OngoingEffectsToJustApplyStart) {

			if (!effect.Get()) continue;

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(effect, 1, EffectContext);

			if (SpecHandle.IsValid()) {

				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				if (!ActiveGEHandle.WasSuccessfullyApplied()) {

					UE_LOG(LogTemp, Error, TEXT("Failed to apply startup effect! %s"), *GetNameSafe(effect))

				}
			}
		}
		if (IsInstantiated()) {

			for (auto effect : OngoingEffectsToRemoveOnEnd) {

				if (!effect.Get()) continue;

				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(effect, 1, EffectContext);

				if (SpecHandle.IsValid()) {

					FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

					if (!ActiveGEHandle.WasSuccessfullyApplied()) {

						UE_LOG(LogTemp, Error, TEXT("Failed to apply runtime effect! %s"), *GetNameSafe(effect))
					}

					else {

						RemoveOnEndEffectHandle.Add(ActiveGEHandle);

					}
				}
			}
		}
	}
}

void UGA_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsInstantiated()) {

		for (auto ActiveHandle : RemoveOnEndEffectHandle) {

			if (ActiveHandle.IsValid()) {
				ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveHandle);
			}
		}
		RemoveOnEndEffectHandle.Empty();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

ANyotaCharacters* UGA_Base::getAvatarNyotaActor()
{
	return 	Cast<ANyotaCharacters>(GetAvatarActorFromActorInfo());
}
