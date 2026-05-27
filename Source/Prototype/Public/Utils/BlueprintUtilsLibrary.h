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

    UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
    static TArray<AActor *> HitBoxOverlapTest(
        UObject *WorldContextObject, AActor *Instigator, float HitBoxRadius, const FVector &ForwardVector,
        float HitBoxElevationOffset, bool bDrawDebugs
    );

    static void DrawDebugInformation(
        UObject *WorldContextObject, const TArray<FOverlapResult> &OverlapResults, const FVector &HitBoxLocation,
        float HitBoxRadius
    );

    UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
    static TArray<FHitResult> SocketSweepTest(
        UObject *WorldContextObject, AActor *Instigator, const FVector &Start, const FVector &End, float Radius,
        const TArray<AActor *> &InIgnoreActors, bool bDrawDebugs
    );
};
