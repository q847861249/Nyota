// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"

#include "NyotaGameMode.generated.h"

class UNyotaExperienceDefinition;
class UNyotaExperienceManagerComponent;
class UNyotaPawnData;

/**
 *
 */
UCLASS(Config = Game)
class NYOTA_API ANyotaGameMode : public AModularGameModeBase
{
    GENERATED_BODY()
public:
    ANyotaGameMode(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Nyota|Pawn")
    const UNyotaPawnData *GetPawnDataForController(const AController *InController) const;

    //~AGameModeBase interface
    virtual APawn *SpawnDefaultPawnAtTransform_Implementation(
        AController *NewPlayer, const FTransform &SpawnTransform
    ) override;

    virtual void InitGameState() override;
    //~End of AGameModeBase interface

protected:
    void OnExperienceLoaded(const UNyotaExperienceDefinition *CurrentExperience);

    const UNyotaExperienceManagerComponent *GetExperienceManagerComponent() const;

    // The default experience to use when no experience is specified via URL or WorldSettings
    UPROPERTY(Config, EditDefaultsOnly, Category = "Experience")
    TSoftObjectPtr<UNyotaExperienceDefinition> DefaultExperience;
};
