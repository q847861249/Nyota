// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_VortexGrip.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/BaseCharacter.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"

void UGA_VortexGrip::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    StartVortexGrip();

    UAbilityTask_WaitGameplayEvent *ApplyDamageEvent =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ApplyDamage);
    ApplyDamageEvent->EventReceived.AddDynamic(this, &ThisClass::ApplyDamage);
    ApplyDamageEvent->ReadyForActivation();
}

void UGA_VortexGrip::StartVortexGrip()
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        return;
    }

    if (IsValid(Player->GetGrabbedEnemy()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Grabbed Target: %s"), *Player->GetGrabbedEnemy()->GetName());

        // Set Thrown Force
        Player->PendingThrownForce = VortexGripForce;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Grabbed Target Is Null !!!"));
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }

    // Play Animation Montage
    UAbilityTask_PlayMontageAndWait *PlayMontage =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("VortexGrip"), VortexGripMontage);
    PlayMontage->OnCompleted.AddDynamic(this, &ThisClass::OnAbilityEnd);
    PlayMontage->OnInterrupted.AddDynamic(this, &ThisClass::OnAbilityEnd);
    PlayMontage->OnCancelled.AddDynamic(this, &ThisClass::OnAbilityEnd);
    PlayMontage->ReadyForActivation();
}

void UGA_VortexGrip::ApplyDamage(FGameplayEventData EventData)
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        return;
    }

    ABaseCharacter *GrabbedEnemy = Player->GetGrabbedEnemy();

    if (!IsValid(GrabbedEnemy))
    {
        return;
    }

    if (!IsValid(GrabbedEnemy->GetAbilitySystemComponent()))
    {
        return;
    }

    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();
    FGameplayEffectSpecHandle SpecHandle =
        ASC->MakeOutgoingSpec(VortexGripEffect, EventData.EventMagnitude, FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GrabbedEnemy->GetAbilitySystemComponent());
}

void UGA_VortexGrip::OnAbilityEnd()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
