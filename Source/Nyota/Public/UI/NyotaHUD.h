// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NyotaHUD.generated.h"

/**
 *
 */
UCLASS(Config = Game)
class NYOTA_API ANyotaHUD : public AHUD
{
    GENERATED_BODY()

public:
    ANyotaHUD(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());
};
