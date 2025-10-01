// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NyotaWeapon.h"
#include "Character/NyotaCharacters.h"
#include "Input/NyotaGameplayTags.h"
#include "Ability/GA/GA_OnHit.h"
#include "Ability/GA/GA_Damage.h"


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

	bReplicates = true;
	SetReplicateMovement(true);


	// Hit event binding
	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ANyotaWeapon::WeaponBeginOverlap);
	WeaponCapsule->OnComponentEndOverlap.AddDynamic(this, &ANyotaWeapon::WeaponEndOverlap);
	WeaponCapsule->OnComponentHit.AddDynamic(this, &ANyotaWeapon::WeaponOnHit);


	//ability system
	AbilitySystem = CreateDefaultSubobject<UNyota_AbilitySystemComponent>(TEXT("GameplayAbilitySystem"));
}

void ANyotaWeapon::WeaponOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void ANyotaWeapon::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!GetInstigator()->HasAuthority())  return;

	if (WeaponType == EWeaponType::Melee) 
	{
		if (OtherActor->HasAuthority() && (OtherActor != GetInstigator()))
		{
			if (ANyotaCharacters* charater = Cast<ANyotaCharacters>(OtherActor))
			{
				if (DamageEffects.IsEmpty()) return;

				for (TSubclassOf<UGameplayEffect> effect : DamageEffects) 
				{
					if (!effect->GetDefaultObject<UGameplayEffect>()) return;

					AbilitySystem->ApplyGameplayEffectToTarget(effect->GetDefaultObject<UGameplayEffect>(), charater->AbilitySystem);

				}
			}
		}
	}
}

void ANyotaWeapon::WeaponEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}



