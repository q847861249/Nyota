// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_SpawnWeapon.h"
#include "Character/NyotaPlayer.h"

void UGA_SpawnWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle,ActorInfo,ActivationInfo,TriggerEventData);


	if (ANyotaCharacters* Character = Cast<ANyotaCharacters>(ActorInfo->AvatarActor))
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Character;
		SpawnInfo.Owner = Character;

		FRotator SpawnTransformRotation = Character->GetRootComponent()->GetRelativeRotation();

		FVector SpawnTransformLocation = Character->GetMesh()->GetSocketLocation(WeaponSpawnSocketName);

		ANyotaWeapon* CurrentWeapon = GetWorld()->SpawnActor<ANyotaWeapon>(WeaponBlueprintClass, SpawnTransformLocation, SpawnTransformRotation, SpawnInfo);

		if (CurrentWeapon) 
		{
			Character->SetWeapon(CurrentWeapon);

			CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSpawnSocketName);
		} 

	}
}
