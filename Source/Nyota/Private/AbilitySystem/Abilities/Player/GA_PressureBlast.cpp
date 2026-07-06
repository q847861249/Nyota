// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_PressureBlast.h"

#include "NyotaLogChannels.h"
#include "Character/BasePlayer.h"

void UGA_PressureBlast::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!Player)
    {
        return;
    }

    if (Player->GetGrabbedEnemy())
    {
        UE_LOG(LogNyota, Warning, TEXT("Grabbed Target: %s"), *Player->GetGrabbedEnemy()->GetName());
    }
    else
    {
        UE_LOG(LogNyota, Error, TEXT("Grabbed Target Is Null !!!"));
    }
}