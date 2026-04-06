// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Nyota_PlayerState.h"
#include "Components/Nyota_AbilitySystemComponent.h"
#include "AttributeSet/Nyota_AttributeSet.h"
ANyota_PlayerState::ANyota_PlayerState()
{
    //Synchronize once every 0.1 seconds
    NetUpdateFrequency = 100.f;

    AbilitySystemComponent = CreateDefaultSubobject<UNyota_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    // Set Mixed replication mode so that only the necessary data is replicated to clients, reducing bandwidth usage.
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    BaseAttributeSet = CreateDefaultSubobject<UNyota_AttributeSet>(TEXT("BaseAttributeSet"));
}

UAbilitySystemComponent* ANyota_PlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
UAttributeSet* ANyota_PlayerState::GetBaseAttributeSet() const
{
    return BaseAttributeSet;
}


