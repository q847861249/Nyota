// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/Player/GA_Slam.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/BaseCharacter.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"

void UGA_Slam::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    StartSlam();

    UAbilityTask_WaitGameplayEvent *ApplySlamDamage =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ApplyDamage);
    ApplySlamDamage->EventReceived.AddDynamic(this, &ThisClass::OnApplySlamDamage);
    ApplySlamDamage->ReadyForActivation();
}

void UGA_Slam::StartSlam()
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        return;
    }

    if (IsValid(Player->GetGrabbedEnemy()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Grabbed Target: %s"), *Player->GetGrabbedEnemy()->GetName());

        UAbilityTask_PlayMontageAndWait *PlayMontageTask =
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("Slam"), SlamMontage);
        PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnSlamEnd);
        PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnSlamEnd);
        PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnSlamEnd);
        PlayMontageTask->ReadyForActivation();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Grabbed Target Is Null !!!"));
    }
}

void UGA_Slam::OnApplySlamDamage(FGameplayEventData EventData)
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
        ASC->MakeOutgoingSpec(SlamEffect, EventData.EventMagnitude, FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GrabbedEnemy->GetAbilitySystemComponent());
}

void UGA_Slam::OnSlamEnd()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}