// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "NyotaWeapon.generated.h"


UCLASS()
class NYOTA_API ANyotaWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANyotaWeapon();

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UCapsuleComponent* WeaponCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;


};
