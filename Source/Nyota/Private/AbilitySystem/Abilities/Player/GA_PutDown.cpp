// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_PutDown.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/BaseEnemyWildBoar.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"

UGA_PutDown::UGA_PutDown(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    AbilityTags.AddTag(Nyota::Ability_Grab_PutDown);
    ActivationRequiredTags.AddTag(Nyota::Ability_State_Grabbing_PutDownWindow);
}

void UGA_PutDown::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

        return;
    }

    ABaseEnemyWildBoar *WildBoar = Cast<ABaseEnemyWildBoar>(Player->GetGrabbedEnemy());
    const bool bReleasedTarget = IsValid(WildBoar);

    if (bReleasedTarget)
    {
        // 放下，不添加冲力
        // WildBoar->OnBoarReleased();

        // 冲力抛出
        WildBoar->OnThrown(
            GetAvatarActorFromActorInfo()->GetActorForwardVector(),
            Player->PendingThrownForce > 0.f ? Player->PendingThrownForce : ThrownForce
        );
    }

    Player->PendingThrownForce = -1.f;
    Player->ResetGrabbedEnemy();

    FGameplayEventData Payload;
    Payload.Instigator = Player;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, Nyota::Event_Ability_PutDownEnd, Payload);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, !bReleasedTarget);
}
