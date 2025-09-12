// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_Base.h"
#include "Character/NyotaCharacters.h"
#include "Debug/Debug.h"
#include <AbilitySystemGlobals.h>

void UGA_Base::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivatePolicy == EAbilityActivatePolicy::Immediate)
	{
		if (ActorInfo && !Spec.IsActive()) 
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		
		}
	}
}

bool UGA_Base::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;

	const ANyotaCharacters* Character = CastChecked<ANyotaCharacters>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	return true;
}

void UGA_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbilityCooldown()) 
	{
		K2_EndAbility();
		return;
	}

		
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

		for (auto effect : OngoingEffectsToJustApplyStart) {

			if (!effect.Get()) continue;

			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, effect, 1);

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

				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, effect, 1);

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
	if (IsInstantiated()) 
	{
		if (AbilityActivatePolicy == EAbilityActivatePolicy::Immediate)
		{
			if (ActorInfo)
			{
				ActorInfo->AbilitySystemComponent->TryActivateAbility(Handle);

			}
		}


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

 TArray<FActiveGameplayEffectHandle> UGA_Base::ApplyEffectContainerSpec(const FGSCGameplayEffectContainerSpec& ContainerSpec)
 {
	 TArray<FActiveGameplayEffectHandle> AllEffects;

	 // Iterate list of effect specs and apply them to their target data
	 for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	 {
		 AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	 }
	 return AllEffects;
 }

 TArray<FActiveGameplayEffectHandle> UGA_Base::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
 {
	 const FGSCGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	 return ApplyEffectContainerSpec(Spec);
 }

 FGSCGameplayEffectContainerSpec UGA_Base::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
 {
	 FGSCGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	 if (FoundContainer)
	 {
		 return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	 }
	 return FGSCGameplayEffectContainerSpec();
 }

 FGSCGameplayEffectContainerSpec UGA_Base::MakeEffectContainerSpecFromContainer(const FGSCGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
 {
	 // First figure out our actor info
	 FGSCGameplayEffectContainerSpec ReturnSpec;
	 const AActor* OwningActor = GetOwningActorFromActorInfo();
	 UAbilitySystemComponent* OwningASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);

	 if (OwningASC)
	 {

		 // If we don't have an override level, use the default on the ability itself
		 if (OverrideGameplayLevel == INDEX_NONE)
		 {
			 OverrideGameplayLevel = GetAbilityLevel();
		 }

		 // Build GameplayEffectSpecs for each applied effect
		 for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		 {
			 FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel);

			 FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
			 if (Spec && Container.bUseSetByCallerMagnitude)
			 {
				 Spec->SetSetByCallerMagnitude(Container.SetByCallerDataTag, Container.SetByCallerMagnitude);
			 }
			 ReturnSpec.TargetGameplayEffectSpecs.Add(SpecHandle);
		 }
	 }
	 return ReturnSpec;
 }
