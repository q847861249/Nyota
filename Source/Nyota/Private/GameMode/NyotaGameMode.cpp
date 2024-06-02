// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NyotaGameMode.h"
#include "Character/NyotaPlayerController.h"
#include "Character/NyotaPlayerController.h"

ANyotaGameMode::ANyotaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NyotaCharacter/Gemini/Blueprint/BP_Gemini"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//ReplaySpectatorPlayerControllerClass = ANyotaPlayerController::StaticClass();

	PlayerControllerClass = ANyotaPlayerController::StaticClass();
}

void ANyotaGameMode::NotifyPlayerDied(ANyotaPlayerController* PlayerController)
{
	if (PlayerController) {

		PlayerController->ResartPlayerin(2.f);

	}
}
