// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnComponent.h"

#include "NyotaPawnComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class NYOTA_API UNyotaPawnComponent : public UPawnComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UNyotaPawnComponent(const FObjectInitializer &ObjectInitializer);

protected:
    void Input_AbilityInputTagPressed(FGameplayTag InputTag);

    void Input_AbilityInputTagReleased(FGameplayTag InputTag);
};
