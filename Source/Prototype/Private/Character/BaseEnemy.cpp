// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseEnemy.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "GA/CustomAbilitySystemComponent.h"

ABaseEnemy::ABaseEnemy()
{
    PrimaryActorTick.bCanEverTick = false;

    AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>("AbilitySystemComponent");
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

    if (!HasAuthority())
    {
        return;
    }

    GiveDefaultAbility();

    InitializeAttributes();
}

UAbilitySystemComponent *ABaseEnemy::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
