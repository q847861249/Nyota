// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/Player/GA_WaterBubble.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Actor/BaseProjectile.h"
#include "Character/BaseCharacter.h"
#include "GameplayTags/GameTags.h"

void UGA_WaterBubble::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    bIsStopping = false;

    UAbilityTask_PlayMontageAndWait *PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("StartMontage"), StartMontage);
    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
    PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageCompleted);
    PlayMontageTask->ReadyForActivation();
}

void UGA_WaterBubble::OnMontageCompleted()
{
    UWorld *World = GetWorld();
    if (!IsValid(GetWorld()))
    {
        return;
    }

    UAbilityTask_PlayMontageAndWait *Task =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("LoopMontage"), LoopMontage);
    Task->ReadyForActivation();

    // Set Timer
    World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SpawnWaterBubble, 0.15f, true);

    // Wait Event
    UAbilityTask_WaitGameplayEvent *WaitGameplayEventTask =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_WaterBubbleEnd);
    WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnWaterBubbleEnd);
    WaitGameplayEventTask->ReadyForActivation();
}

void UGA_WaterBubble::OnEndMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_WaterBubble::SpawnWaterBubble()
{
    if (bIsStopping)
    {
        return;
    }

    ABaseCharacter *BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if (!IsValid(BaseCharacter))
    {
        return;
    }

    UWorld *World = GetWorld();
    if (!IsValid(GetWorld()))
    {
        return;
    }

    FVector FireLocation = BaseCharacter->GetMesh()->GetSocketLocation(FireSockName);
    FRotator ControlRotation = BaseCharacter->GetControlRotation();
    FTransform SpawnTransform(ControlRotation, FireLocation, FVector(1.0f, 1.0f, 1.0f));

    FActorSpawnParameters Params;
    Params.Owner = BaseCharacter;
    Params.Instigator = BaseCharacter;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

    ABaseProjectile *WaterBubbleActor = World->SpawnActor<ABaseProjectile>(WaterBubbleClass, SpawnTransform, Params);
    WaterBubbleActor->OnProjectileHit.AddDynamic(this, &ThisClass::OnWaterBubbleHit);
}

void UGA_WaterBubble::OnWaterBubbleEnd(FGameplayEventData EventData)
{
    UWorld *World = GetWorld();
    if (!IsValid(GetWorld()))
    {
        return;
    }

    bIsStopping = true;

    World->GetTimerManager().ClearTimer(TimerHandle);

    MontageStop();

    UAbilityTask_PlayMontageAndWait *Task =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("EndMontage"), EndMontage);
    Task->OnCompleted.AddDynamic(this, &ThisClass::OnEndMontageCompleted);
    Task->OnBlendOut.AddDynamic(this, &ThisClass::OnEndMontageCompleted);
    Task->OnCancelled.AddDynamic(this, &ThisClass::OnEndMontageCompleted);
    Task->OnInterrupted.AddDynamic(this, &ThisClass::OnEndMontageCompleted);
    Task->ReadyForActivation();
}

void UGA_WaterBubble::OnWaterBubbleHit(const FHitResult &HitResult)
{
    ABaseCharacter *HitActor = Cast<ABaseCharacter>(HitResult.GetActor());
    if (!IsValid(HitActor))
    {
        return;
    }

    if (!IsValid(HitActor->GetAbilitySystemComponent()))
    {
        return;
    }

    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayEventData PayloadData;
    PayloadData.Instigator = GetAvatarActorFromActorInfo();
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, Nyota::Event_Enemy_HitReact, PayloadData);

    FGameplayEffectSpecHandle SpecHandle =
        ASC->MakeOutgoingSpec(HitEffectClass, GetAbilityLevel(), FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitActor->GetAbilitySystemComponent());
}