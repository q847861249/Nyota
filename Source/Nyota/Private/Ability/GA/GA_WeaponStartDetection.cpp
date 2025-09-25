// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_WeaponStartDetection.h"
#include "Character/NyotaCharacters.h"
#include "Debug/Debug.h"

void UGA_WeaponStartDetection::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ANyotaCharacters* Character = CastChecked<ANyotaCharacters>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	Debug::SLOG("working");

	if (Character->getWeapon())
	{
		Character->getWeapon()->WeaponCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}
