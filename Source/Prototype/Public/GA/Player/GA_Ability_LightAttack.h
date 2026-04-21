// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseSkill.h"
#include "GA_Ability_LightAttack.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API UGA_Ability_LightAttack : public UGA_BaseSkill
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "OverlapTest")
    TArray<AActor *> HitBoxOverlapTest();

    UFUNCTION(BlueprintCallable, Category = "OverlapTest")
    void SendHitReactEventToActor(const TArray<AActor *> &HitActors);

    UFUNCTION(BlueprintCallable, Category = "OverlapTest")
    void SetLookAtEnemyRotation(AActor *LookAtActor) const;

private:
    void DrawDebugInformation(const TArray<FOverlapResult> &OverlapResults, const FVector &HitBoxLocation) const;

    UPROPERTY(EditDefaultsOnly, Category = "OverlapTest")
    float HitBoxRadius = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "OverlapTest")
    float HitBoxForwardOffset = 0.f;

    UPROPERTY(EditDefaultsOnly, Category = "OverlapTest")
    float HitBoxElevationOffset = 20.f;
};
