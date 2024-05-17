// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSystem/EffectAbility.h"
#include "Damage001EffectAbility.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UDamage001EffectAbility : public UEffectAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateCppAbility() override;

	virtual void EndCppAbility() override;
public:
	AActor* MoveActor;

public:
	UPROPERTY(BlueprintReadWrite, Category = "EffectParam")
	FVector Direction;//Length°üº¬ËÙ¶È

	UPROPERTY(BlueprintReadWrite, Category = "EffectParam")
	FVector Position;

	UPROPERTY(BlueprintReadWrite, Category = "EffectParam")
	FVector Scale = FVector(1, 1, 1);

public:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
