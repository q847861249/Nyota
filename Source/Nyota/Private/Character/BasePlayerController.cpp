// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayerController.h"

#include "Character/BasePlayerState.h"
#include "GA/NyotaAbilitySystemComponent.h"

ABasePlayerState *ABasePlayerController::GetNyotaPlayerState() const
{
    return CastChecked<ABasePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UNyotaAbilitySystemComponent *ABasePlayerController::GetNyotaAbilitySystemComponent() const
{
    const ABasePlayerState *NyotaPlayerState = GetNyotaPlayerState();

    return NyotaPlayerState ? NyotaPlayerState->GetNyotaAbilitySystemComponent() : nullptr;
}

void ABasePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
    if (UNyotaAbilitySystemComponent *NyotaASC = GetNyotaAbilitySystemComponent())
    {
        NyotaASC->ProcessAbilityInput(DeltaTime, bGamePaused);
    }

    Super::PostProcessInput(DeltaTime, bGamePaused);
}
