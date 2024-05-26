// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "DataAsset/NyotaCharacterConfig.h"
#include "Projectile/ProjectileBase.h"

#include "NyotaComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NYOTA_API UNyotaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	


	// Sets default values for this component's properties
	UNyotaComponent();



//*****************************//
//** 预设的GA 和 GE 必须要填 **//
// ****************************//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterDefaultConfig, Replicated)
	UNyotaCharacterConfig* CharacterConfig;

public:

	//get character weapon instance
	//return null when not owning weapon
	UFUNCTION(BlueprintCallable)
	ANyotaWeapon* GetWeaponInstance();

public:

	//get character onhit projectile information
	UPROPERTY(BlueprintReadOnly, Replicated)
	FProjectileCFG CurrentActivateProjectileInfo;

protected:
	// current on hand weapon
	UPROPERTY(Replicated)
	ANyotaWeapon* CurrentWeapon;

private:
	// read the config file and initilize the weapon
	bool InitializeWeapon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};

