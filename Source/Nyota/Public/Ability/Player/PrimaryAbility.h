// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "PrimaryAbility.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UPrimaryAbility : public UBaseAbility
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	float HitBoxForwardOffset = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	float HitBoxElevationOffset = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "HitBox")
	float HitBoxRadius = 100.f;

	void DrawHitBoxOverlapDebugs(const TArray<AActor*>& FilterActor, const FVector& HitBoxLocation);

	UFUNCTION(BlueprintCallable)
	void SendHitReactEventToActor(TArray<AActor*> HitActors);
public:
	UFUNCTION(BlueprintCallable)
	void PrimaryAttack();
};
