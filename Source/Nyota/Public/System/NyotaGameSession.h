// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "NyotaGameSession.generated.h"

UCLASS(Config = Game)
class NYOTA_API ANyotaGameSession : public AGameSession
{
    GENERATED_BODY()

public:
    ANyotaGameSession(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());
};
