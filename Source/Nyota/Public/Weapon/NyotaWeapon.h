// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "Component/Nyota_AbilitySystemComponent.h"
#include "NyotaWeapon.generated.h"



UENUM(BlueprintType)
enum class EWeaponType : uint8 {Melee, Ranged};


UCLASS()
class NYOTA_API ANyotaWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANyotaWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType; 

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities)
	TObjectPtr<UNyota_AbilitySystemComponent> AbilitySystem;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UCapsuleComponent* WeaponCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> DamageEffects;


public:

	// weapon hit event
	UFUNCTION()
	void WeaponOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void WeaponEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
