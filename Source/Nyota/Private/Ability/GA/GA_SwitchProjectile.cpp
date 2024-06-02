// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_SwitchProjectile.h"
#include "Weapon/NyotaShooter.h"
#include "Character/NyotaCharacters.h"
#include "Component/NyotaComponent.h"
#include "Debug/Debug.h"

bool UGA_SwitchProjectile::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_SwitchProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ANyotaShooter* shooter = Cast<ANyotaShooter>(getAvatarNyotaActor()->NyotaComponent->GetWeaponInstance());

	if (!shooter) {
		K2_EndAbility();
		return;
	}

	if (!projectile) {
		K2_EndAbility();
		return;
	}

	shooter->SwitchingProjectile(projectile);

	K2_EndAbility();

}

void UGA_SwitchProjectile::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
