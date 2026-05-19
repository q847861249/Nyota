// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/Player/GA_Slam.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/BaseCharacter.h"
#include "GameplayTags/GameTags.h"

void UGA_Slam::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilityTask_PlayMontageAndWait *PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("Slam"), SlamMontage);
    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnSlamEnd);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnSlamEnd);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnSlamEnd);
    PlayMontageTask->Activate();

    UAbilityTask_WaitGameplayEvent *Task =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_HasGrabbedEnemy);
    Task->EventReceived.AddDynamic(this, &ThisClass::OnSlamEventReceived);
    Task->Activate();

    UAbilityTask_WaitGameplayEvent *ApplySlamDamage =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ApplySlamDamage);
    ApplySlamDamage->EventReceived.AddDynamic(this, &ThisClass::OnApplySlamDamage);
    ApplySlamDamage->Activate();
}

void UGA_Slam::OnSlamEventReceived(FGameplayEventData EventData)
{
    GrabbedCharacter = Cast<ABaseCharacter>(const_cast<AActor *>(EventData.Target.Get()));
    if (GrabbedCharacter.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Grabbed Target: %s"), *GrabbedCharacter->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Grabbed Target Is Null !!!"));
    }
}

void UGA_Slam::OnApplySlamDamage(FGameplayEventData EventData)
{
    if (!GrabbedCharacter.IsValid())
    {
        return;
    }

    if (!IsValid(GrabbedCharacter->GetAbilitySystemComponent()))
    {
        return;
    }

    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();
    FGameplayEffectSpecHandle SpecHandle =
        ASC->MakeOutgoingSpec(SlamEffect, EventData.EventMagnitude, FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GrabbedCharacter->GetAbilitySystemComponent());
}

void UGA_Slam::OnSlamEnd()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}