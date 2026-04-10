// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayerState.h"

#include "GA/CustomAbilitySystemComponent.h"

ABasePlayerState::ABasePlayerState()
{
    NetUpdateFrequency = 100.0f;

    AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent *ABasePlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}