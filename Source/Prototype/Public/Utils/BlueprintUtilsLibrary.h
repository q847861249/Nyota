// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enums/PlayerType.h"

#include "BlueprintUtilsLibrary.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API UBlueprintUtilsLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    static EHitDirection GetHitDirection(const FVector &TargetForward, const FVector &ToInstigator);

    UFUNCTION(BlueprintPure)
    static FName GetHitDirectionName(const EHitDirection &HitDirection);
};
