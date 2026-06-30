// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/NyotaGameState.h"

#include "AbilitySystem/NyotaAbilitySystemComponent.h"
#include "GameModes/NyotaExperienceManagerComponent.h"

ANyotaGameState::ANyotaGameState(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    AbilitySystemComponent =
        ObjectInitializer.CreateDefaultSubobject<UNyotaAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    ExperienceManagerComponent = ObjectInitializer.CreateDefaultSubobject<UNyotaExperienceManagerComponent>(
        this, TEXT("ExperienceManagerComponent")
    );
}

UAbilitySystemComponent *ANyotaGameState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
