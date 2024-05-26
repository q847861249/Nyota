// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NyotaWeapon.h"

// Sets default values
ANyotaWeapon::ANyotaWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCapsule"));
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMEsh"));

	WeaponCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WeaponMesh->SetupAttachment(WeaponCapsule);
	RootComponent = WeaponCapsule;
}




