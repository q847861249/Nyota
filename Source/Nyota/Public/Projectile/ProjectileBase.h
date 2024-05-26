// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DataAsset/ProjectileConfig.h"

#include "ProjectileBase.generated.h"

UCLASS()
class NYOTA_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();


protected:

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	AActor* Target;

	UPROPERTY(VisibleDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjetileMesh;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UProjectileConfig* ProjectileConfigFile;



private:
	//read information from the config file
	bool InitilizeConfigStruct();

	UPROPERTY()
	FProjectileCFG ProjectileConfigStruct;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile hit event
	UFUNCTION()
	void ProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void ProjectileEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ProjectileOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Replicated
public:
	UFUNCTION(Reliable,NetMulticast)
	void PlayEffect_Multicast();


};
