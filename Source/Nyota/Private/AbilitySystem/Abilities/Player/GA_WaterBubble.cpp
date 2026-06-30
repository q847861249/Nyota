// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_WaterBubble.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
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
    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnStartMontageCompleted);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnStartMontageInterrupted);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnStartMontageInterrupted);
    PlayMontageTask->ReadyForActivation();
}

void UGA_WaterBubble::OnStartMontageCompleted()
{
    UWorld *World = GetWorld();
    if (!IsValid(GetWorld()))
    {
        return;
    }

    UAbilityTask_PlayMontageAndWait *PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("LoopMontage"), LoopMontage);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnStartMontageInterrupted);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnStartMontageInterrupted);
    PlayMontageTask->ReadyForActivation();

    // Set Timer
    World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SpawnWaterBubble, 0.15f, true);

    // Release Event Listen
    UAbilityTask_WaitInputRelease *WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
    WaitReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnWaterBubbleEnd);
    WaitReleaseTask->ReadyForActivation();
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

void UGA_WaterBubble::OnStartMontageInterrupted()
{
    OnWaterBubbleEnd(0.f);
}

void UGA_WaterBubble::OnWaterBubbleEnd(float TimeHeld)
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
