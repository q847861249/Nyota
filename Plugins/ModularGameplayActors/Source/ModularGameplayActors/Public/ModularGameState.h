// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "ModularGameState.generated.h"

/** Pair this with a ModularGameModeBase */
UCLASS(Blueprintable)
class MODULARGAMEPLAYACTORS_API AModularGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    //~ Begin AActor interface
    virtual void PreInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //~ End AActor interface
};

/** Pair this with a ModularGameMode */
UCLASS(Blueprintable)
class MODULARGAMEPLAYACTORS_API AModularGameState : public AGameState
{
    GENERATED_BODY()

public:
    //~ Begin AActor interface
    virtual void PreInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //~ End AActor interface

protected:
    //~ Begin AGameState interface
    virtual void HandleMatchHasStarted() override;
    //~ Begin AGameState interface
};
