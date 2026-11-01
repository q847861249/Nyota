// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Common/GA_Jump.h"
#include "Character/BasePlayer.h"
void UGA_Jump::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) 
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ABasePlayer* Player = Cast<ABasePlayer>(ActorInfo->AvatarActor.Get());
    if(!Player) return;
    Player->Jump();
    EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
}

void UGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    //逻辑写在前面
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}