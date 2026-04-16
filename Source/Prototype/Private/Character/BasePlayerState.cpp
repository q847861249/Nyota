// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayerState.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "GA/CustomAbilitySystemComponent.h"

ABasePlayerState::ABasePlayerState()
{
    NetUpdateFrequency = 100.0f;

    AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

UAbilitySystemComponent *ABasePlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAttributeSet * ABasePlayerState::GetAttributeSet() const
{
    return AttributeSet;
}