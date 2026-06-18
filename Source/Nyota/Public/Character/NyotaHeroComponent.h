// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "NyotaHeroComponent.generated.h"

struct FInputMappingContextAndPriority;
struct FGameplayTag;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYOTA_API UNyotaHeroComponent : public UPawnComponent
{
    GENERATED_BODY()

public:
    UNyotaHeroComponent(const FObjectInitializer &ObjectInitializer);

protected:
    void InitializePlayerInput(UInputComponent *PlayerInputComponent);

    void Input_AbilityInputTagPressed(const FGameplayTag &InputTag);

    void Input_AbilityInputTagReleased(const FGameplayTag &InputTag);

    UPROPERTY(EditAnywhere)
    TArray<FInputMappingContextAndPriority> DefaultInputMappings;
};
