// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseSkill.h"
#include "GA_HitReact.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UGA_HitReact : public UGA_BaseSkill
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Crash | Abilities")
    void CacheHitDirectionVectors(AActor *Instigator);

    UPROPERTY(BlueprintReadOnly, Category = "Crash | Abilities")
    FVector AvatarForward;

    UPROPERTY(BlueprintReadOnly, Category = "Crash | Abilities")
    FVector ToInstigator;
};
