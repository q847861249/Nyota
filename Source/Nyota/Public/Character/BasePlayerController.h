// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABasePlayerState;
class UNyotaAbilitySystemComponent;
/**
 *
 */
UCLASS(Config = Game)
class NYOTA_API ABasePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABasePlayerState *GetNyotaPlayerState() const;

    UNyotaAbilitySystemComponent *GetNyotaAbilitySystemComponent() const;

    virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};

UCLASS()
class ABaseReplayPlayerController : public ABasePlayerController
{
    GENERATED_BODY()
};
