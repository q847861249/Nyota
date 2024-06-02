// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NyotaPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Input/NyotaGameplayTags.h"
#include "GameMode/NyotaGameMode.h"



void ANyotaPlayerController::ResartPlayerin(float InTime)
{
	ChangeState(NAME_Spectating);

	GetWorld()->GetTimerManager().SetTimer(RestartPlayerTimerHandle,this, &ANyotaPlayerController::RestartPlayer, InTime, false);
}

void ANyotaPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(aPawn)) {

		DeathStateTagDelegate = AbilityComponent->RegisterGameplayTagEvent(FNyotaGameplayTags::Get().State_Dead, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ANyotaPlayerController::OnPawnDeathStateChanged);
	
	}
}

void ANyotaPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (DeathStateTagDelegate.IsValid()) {

		if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn())) {

			AbilityComponent->UnregisterGameplayTagEvent(DeathStateTagDelegate, FNyotaGameplayTags::Get().State_Dead, EGameplayTagEventType::NewOrRemoved);

		}
	}
}

void ANyotaPlayerController::OnPawnDeathStateChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0) {

		UWorld* world = GetWorld();

		ANyotaGameMode* GameMode = world ? Cast<ANyotaGameMode>(world->GetAuthGameMode()) : nullptr;

		if (GameMode) {
		
			GameMode->NotifyPlayerDied(this);
		}

		if (DeathStateTagDelegate.IsValid()) {

			if (UAbilitySystemComponent* AbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn())) {

				AbilityComponent->UnregisterGameplayTagEvent(DeathStateTagDelegate, FNyotaGameplayTags::Get().State_Dead, EGameplayTagEventType::NewOrRemoved);

			}
		}
	}
}

void ANyotaPlayerController::RestartPlayer()
{
	UWorld* world = GetWorld();

	ANyotaGameMode* GameMode = world ? Cast<ANyotaGameMode>(world->GetAuthGameMode()) : nullptr;

	if (GameMode) {
	
		GameMode->RestartPlayer(this);
	
	}
}
