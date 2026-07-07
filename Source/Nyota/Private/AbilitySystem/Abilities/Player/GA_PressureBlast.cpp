// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_PressureBlast.h"

#include "AbilitySystemComponent.h"
#include "NyotaLogChannels.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"

void UGA_PressureBlast::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    Execute();
}

void UGA_PressureBlast::Execute()
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!IsValid(Player))
    {
        return;
    }

    if (!IsValid(Player->GetGrabbedEnemy()))
    {
        // UE_LOG(LogNyota, Warning, TEXT("UGA_PressureBlast: No grabbed enemy, ending ability."));

        Terminate();

        return;
    }

    UE_LOG(LogNyota, Warning, TEXT("UGA_PressureBlast: Pressure Blast on %s"), *Player->GetGrabbedEnemy()->GetName());

    Player->PendingThrownForce = PressureBlastForce;

    UAbilityTask_WaitGameplayEvent *ApplyDamage =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ApplyDamage);
    ApplyDamage->EventReceived.AddDynamic(this, &ThisClass::OnApplyDamage);
    ApplyDamage->ReadyForActivation();

    UAbilityTask_PlayMontageAndWait *PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this, FName("PressureBlast"), PressureBlastMontage
    );
    PlayMontage->OnCompleted.AddDynamic(this, &ThisClass::Terminate);
    PlayMontage->OnInterrupted.AddDynamic(this, &ThisClass::Terminate);
    PlayMontage->OnCancelled.AddDynamic(this, &ThisClass::Terminate);
    PlayMontage->ReadyForActivation();
}

void UGA_PressureBlast::Terminate()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_PressureBlast::OnApplyDamage(FGameplayEventData Payload)
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!Player)
    {
        return;
    }

    ABaseCharacter *GrabbedEnemy = Player->GetGrabbedEnemy();

    if (!GrabbedEnemy)
    {
        return;
    }

    if (!GrabbedEnemy->GetAbilitySystemComponent())
    {
        return;
    }

    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();
    FGameplayEffectSpecHandle SpecHandle =
        ASC->MakeOutgoingSpec(PressureBlastEffect, Payload.EventMagnitude, FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GrabbedEnemy->GetAbilitySystemComponent());
}