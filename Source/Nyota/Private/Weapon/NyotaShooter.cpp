// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NyotaShooter.h"
#include "Character/NyotaCharacters.h"
#include "Net/UnrealNetwork.h"
#include "Debug/Debug.h"



ANyotaShooter::ANyotaShooter()
{
	bReplicates = true;
	//InitilizeProjectile();
}

void ANyotaShooter::BeginPlay()
{
	Super::BeginPlay();

	InitilizeProjectile();

}

void ANyotaShooter::InitilizeProjectile()
{
	if (TSubclassOf<AProjectileBase> projectile_copy = Projectiles[0]) {

		CurrentProjectile = projectile_copy;
	}
	else {
		Debug::SLOG("InitilizeProjectile faild!");

	}
}

void ANyotaShooter::SwitchingProjectile_Server_Implementation(TSubclassOf<AProjectileBase> projectile)
{
	SwitchingProjectile_Multicast(projectile);
}

void ANyotaShooter::SwitchingProjectile_Multicast_Implementation(TSubclassOf<AProjectileBase> projectile)
{
	for (auto& projectile_copy : Projectiles) {

		if (projectile_copy == projectile) {

			CurrentProjectile = projectile_copy;
		}
	}
}

void ANyotaShooter::LaunchProjectile_Server_Implementation(FName SocketName)
{
	LaunchProjectile_Multicast(SocketName);
}

void ANyotaShooter::LaunchProjectile_Multicast_Implementation(FName SocketName)
{
	if (CurrentProjectile) {

		// check the instigator is NyotaCharacter
		if (ANyotaCharacters* Character = Cast<ANyotaCharacters>(GetInstigator())) {

			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Instigator = GetInstigator();
			SpawnInfo.Owner = this;

			//Launch Rotation, might need to change to spring arm component
			FRotator SpawnTransformRotation = Character->GetRootComponent()->GetRelativeRotation();
			FVector SpawnTransformLocation = GetActorLocation();

			GetWorld()->SpawnActor<AProjectileBase>(CurrentProjectile, SpawnTransformLocation, SpawnTransformRotation, SpawnInfo);
		}
	}
}


void ANyotaShooter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ANyotaShooter, CurrentProjectile, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ANyotaShooter, Projectiles, COND_None, REPNOTIFY_Always);
}
