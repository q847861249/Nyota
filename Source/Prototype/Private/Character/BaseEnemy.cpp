// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseEnemy.h"

#include "Character/BasePlayerState.h"
#include "AbilitySystemComponent.h"

ABaseEnemy::ABaseEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent *ABaseEnemy::GetAbilitySystemComponent() const
{
    ABasePlayerState *MyPlayerState = Cast<ABasePlayerState>(GetPlayerState());
    if (!IsValid(MyPlayerState))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: MyPlayerState Is Null."), *GetNameSafe(this));
        return nullptr;
    }

    return MyPlayerState->GetAbilitySystemComponent();
}

void ABaseEnemy::PossessedBy(AController *NewController)
{
    Super::PossessedBy(NewController);

    if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority())
    {
        return;
    }

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
    
    GiveDefaultAbility();
}

void ABaseEnemy::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    if (!IsValid(GetAbilitySystemComponent()))
    {
        return;
    }

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
}
