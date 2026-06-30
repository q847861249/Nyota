// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayerState.h"

#include "NyotaLogChannels.h"
#include "AbilitySystem/NyotaAbilitySet.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Character/NyotaPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GA/NyotaAbilitySystemComponent.h"
#include "GameModes/NyotaExperienceManagerComponent.h"
#include "GameModes/NyotaGameMode.h"
#include "Net/UnrealNetwork.h"

const FName ABasePlayerState::NAME_NyotaAbilityReady("NyotaAbilitiesReady");

ABasePlayerState::ABasePlayerState(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    NetUpdateFrequency = 100.0f;

    AbilitySystemComponent = CreateDefaultSubobject<UNyotaAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

void ABasePlayerState::PreInitializeComponents()
{
    Super::PreInitializeComponents();
}

void ABasePlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    check(AbilitySystemComponent);
    AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

    UWorld *World = GetWorld();
    if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
    {
        AGameStateBase *GameState = GetWorld()->GetGameState();
        check(GameState);
        UNyotaExperienceManagerComponent *ExperienceComponent =
            GameState->FindComponentByClass<UNyotaExperienceManagerComponent>();
        check(ExperienceComponent);
        ExperienceComponent->CallOrRegister_OnExperienceLoaded(
            FOnNyotaExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded)
        );
    }
}

UNyotaAbilitySystemComponent *ABasePlayerState::GetNyotaAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAbilitySystemComponent *ABasePlayerState::GetAbilitySystemComponent() const
{
    return GetNyotaAbilitySystemComponent();
}

UAttributeSet *ABasePlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

void ABasePlayerState::SetPawnData(const UNyotaPawnData *InPawnData)
{
    check(InPawnData);

    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    if (PawnData)
    {
        UE_LOG(
            LogNyota,
            Error,
            TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."),
            *GetNameSafe(InPawnData),
            *GetNameSafe(this),
            *GetNameSafe(PawnData)
        );
        return;
    }

    MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
    PawnData = InPawnData;

    for (const UNyotaAbilitySet *AbilitySet : PawnData->AbilitySets)
    {
        if (AbilitySet)
        {
            AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
        }
    }

    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_NyotaAbilityReady);

    ForceNetUpdate();
}

void ABasePlayerState::OnRep_PawnData()
{
}

void ABasePlayerState::OnExperienceLoaded(const UNyotaExperienceDefinition *CurrentExperience)
{
    if (ANyotaGameMode *NyotaGameMode = GetWorld()->GetAuthGameMode<ANyotaGameMode>())
    {
        if (const UNyotaPawnData *NewPawnData = NyotaGameMode->GetPawnDataForController(GetOwningController()))
        {
            SetPawnData(NewPawnData);
        }
        else
        {
            UE_LOG(
                LogNyota,
                Error,
                TEXT(
                    "ANyotaPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"
                ),
                *GetNameSafe(this)
            );
        }
    }
}

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    FDoRepLifetimeParams SharedParams;
    SharedParams.bIsPushBased = true;

    DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}