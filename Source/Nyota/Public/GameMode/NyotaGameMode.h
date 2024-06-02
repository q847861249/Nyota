// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NyotaGameMode.generated.h"


class ANyotaPlayerController;

UCLASS()
class NYOTA_API ANyotaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANyotaGameMode();

	void NotifyPlayerDied(ANyotaPlayerController* PlayerController);

};
