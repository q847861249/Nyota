// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NyotaComponent.h"
#include "Character/NyotaCharacters.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/NyotaShooter.h"
#include "Weapon/NyotaMelee.h"


// Sets default values for this component's properties
UNyotaComponent::UNyotaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UNyotaComponent::BeginPlay()
{
	Super::BeginPlay();

	if (InitializeWeapon()) {};
}


// Called every frame
void UNyotaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UNyotaComponent::InitializeWeapon()
{
	if (CharacterConfig->NyotaWeapon.Num() == 0) return false;

	for (auto weapon : CharacterConfig->NyotaWeapon) {

		if (ANyotaCharacters* Character = Cast<ANyotaCharacters>(GetOwner())) {

			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Instigator = Character;
	
			FRotator SpawnTransformRotation = Character->GetRootComponent()->GetRelativeRotation();

			FVector SpawnTransformLocation = Character->GetMesh()->GetSocketLocation(weapon.SocketName);

			CurrentWeapon = GetWorld()->SpawnActor<ANyotaWeapon>(weapon.NyotaWeapon, SpawnTransformLocation, SpawnTransformRotation, SpawnInfo);

			if (CurrentWeapon) CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weapon.SocketName);
		
			break;
		}
	}

	return CurrentWeapon->IsValidLowLevel();
}


ANyotaWeapon* UNyotaComponent::GetWeaponInstance()
{
	if (ANyotaShooter* shooter = Cast<ANyotaShooter>(CurrentWeapon)) return shooter;
	else if (ANyotaMelee* melee = Cast<ANyotaMelee>(CurrentWeapon)) return melee;
	else return nullptr;
}


void UNyotaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, CharacterConfig, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, CurrentActivateProjectileInfo, COND_None, REPNOTIFY_Always);
	
}
