// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class MODULARGAMEPLAYACTORS_API AModularPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    //~ Begin AActor interface
    virtual void PreInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Reset() override;
    //~ End AActor interface

protected:
    //~ Begin APlayerState interface
    virtual void CopyProperties(APlayerState *PlayerState) override;
    //~ End APlayerState interface
};
