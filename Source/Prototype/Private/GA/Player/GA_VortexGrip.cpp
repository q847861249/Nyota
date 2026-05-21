// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/Player/GA_VortexGrip.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/BaseCharacter.h"
#include "GameplayTags/GameTags.h"

void UGA_VortexGrip::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilityTask_PlayMontageAndWait *PlayMontage =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("VortexGrip"), VortexGripMontage);
    PlayMontage->OnCompleted.AddDynamic(this, &ThisClass::OnAbilityEnd);
    PlayMontage->OnInterrupted.AddDynamic(this, &ThisClass::OnAbilityEnd);
    PlayMontage->OnCancelled.AddDynamic(this, &ThisClass::OnAbilityEnd);
    PlayMontage->Activate();

    UAbilityTask_WaitGameplayEvent *Task =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_HasGrabbedEnemy);
    Task->EventReceived.AddDynamic(this, &ThisClass::OnEventReceived);
    Task->Activate();

    UAbilityTask_WaitGameplayEvent *ApplyDamageEvent =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ApplyDamage);
    ApplyDamageEvent->EventReceived.AddDynamic(this, &ThisClass::ApplyDamage);
    ApplyDamageEvent->Activate();
}

void UGA_VortexGrip::OnEventReceived(FGameplayEventData EventData)
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

void UGA_VortexGrip::ApplyDamage(FGameplayEventData EventData)
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
        ASC->MakeOutgoingSpec(VortexGripEffect, EventData.EventMagnitude, FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GrabbedCharacter->GetAbilitySystemComponent());
}

void UGA_VortexGrip::OnAbilityEnd()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
