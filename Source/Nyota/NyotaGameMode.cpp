// Copyright Epic Games, Inc. All Rights Reserved.

#include "NyotaGameMode.h"
#include "./Character/NyotaCharacters.h"
#include "UObject/ConstructorHelpers.h"

ANyotaGameMode::ANyotaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NyotaCharacter/Gemini/Blueprint/BP_Gemini"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
