// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NyotaPawnComponent.h"

#include "Character/NyotaPawnExtensionComponent.h"

UNyotaPawnComponent::UNyotaPawnComponent(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNyotaPawnComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
    const APawn *Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return;
    }

    const UNyotaPawnExtensionComponent *PawnExtensionComponent =
        UNyotaPawnExtensionComponent::FindPawnExtensionComponent(Pawn);
    if (!PawnExtensionComponent)
    {
    }
}

void UNyotaPawnComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
}
