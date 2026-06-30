// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseEnemy.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "GA/NyotaAbilitySystemComponent.h"

ABaseEnemy::ABaseEnemy()
{
    PrimaryActorTick.bCanEverTick = false;

    AbilitySystemComponent = CreateDefaultSubobject<UNyotaAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

void ABaseEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (!IsValid(AbilitySystemComponent))
    {
        return;
    }

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

    if (!HasAuthority())
    {
        return;
    }

    InitializeAttributes();

    UBaseAttributeSet *BaseAttributeSet = Cast<UBaseAttributeSet>(GetAttributeSet());
    if (!IsValid(BaseAttributeSet))
    {
        return;
    }

    GetAbilitySystemComponent()
        ->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute())
        .AddUObject(this, &ThisClass::OnHealthChange);
}

UAbilitySystemComponent *ABaseEnemy::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAttributeSet *ABaseEnemy::GetAttributeSet() const
{
    return AttributeSet;
}