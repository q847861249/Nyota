// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NyotaExperienceActionSet.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UNyotaExperienceActionSet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // List of Game Feature Plugins this experience wants to have active
    UPROPERTY(EditAnywhere, Category = "Feature Dependencies")
    TArray<FString> GameFeaturesToEnable;
};
