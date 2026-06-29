// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NyotaExperienceDefinition.generated.h"

class UNyotaExperienceActionSet;
class UNyotaPawnData;
/**
 * Definition of an experience
 */
UCLASS(BlueprintType, Const)
class NYOTA_API UNyotaExperienceDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UNyotaExperienceDefinition();

    // List of Game Feature Plugins this experience wants to have active
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    TArray<FString> GameFeaturesToEnable;

    /** The default pawn class to spawn for players */
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    TObjectPtr<const UNyotaPawnData> DefaultPawnData;

    // List of additional action sets to compose into this experience
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    TArray<TObjectPtr<UNyotaExperienceActionSet>> ActionSets;
};
