// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseEnemy.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "AbilitySystem/NyotaAbilitySystemComponent.h"

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
    InitStartingAbilities();
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

void ABaseEnemy::InitStartingAbilities()
{
    if(!GetAbilitySystemComponent()) return;
	if(StartingAbilities.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Starting Abilities set on %s"), *GetName());
		return;
	}
	for(const auto& Ability: StartingAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}

UAbilitySystemComponent *ABaseEnemy::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAttributeSet *ABaseEnemy::GetAttributeSet() const
{
    return AttributeSet;
}