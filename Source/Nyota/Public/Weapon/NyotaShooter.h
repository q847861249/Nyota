// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/NyotaWeapon.h"
#include "Projectile/ProjectileBase.h"

#include "NyotaShooter.generated.h"


/**
 * 
 */
UCLASS()
class NYOTA_API ANyotaShooter : public ANyotaWeapon
{
	GENERATED_BODY()
public:

	//static data from weapon property
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated)
	TArray<TSubclassOf<AProjectileBase>> Projectiles;

	//Current Active Projectile 
	UPROPERTY(Replicated)
	TSubclassOf<AProjectileBase> CurrentProjectile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ANyotaShooter();

public:

	void InitilizeProjectile();

	// Scokect for lunching projectile sokcet name, 
	// if the socket can not be found, it will return the component world location 
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void LaunchProjectile_Server(FName SocketName);

	UFUNCTION(Reliable, NetMulticast)
	void LaunchProjectile_Multicast(FName SocketName);

	// switching weapon, the function will check projectile array from the config file, otherwise it would not changed
	UFUNCTION(BlueprintCallable,Reliable, NetMulticast)
	void SwitchingProjectile_Server(TSubclassOf<AProjectileBase> projectile);

	UFUNCTION(Reliable, NetMulticast)
	void SwitchingProjectile_Multicast(TSubclassOf<AProjectileBase> projectile);

};
