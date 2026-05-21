// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/Player/GA_Grab.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/BaseEnemyWildBoar.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "Utils/BlueprintUtilsLibrary.h"

void UGA_Grab::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilityTask_PlayMontageAndWait *PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("Grab"), GrabMontage);
    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnGrabTimeout);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnGrabTimeout);
    PlayMontageTask->Activate();

    UAbilityTask_WaitGameplayEvent *WaitSlam =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_GrabSlam);
    WaitSlam->EventReceived.AddDynamic(this, &ThisClass::OnFollowUpInputReceived);
    WaitSlam->Activate();

    UAbilityTask_WaitGameplayEvent *WaitVortexGrip =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_GrabVortexGrip);
    WaitVortexGrip->EventReceived.AddDynamic(this, &ThisClass::OnFollowUpInputReceived);
    WaitVortexGrip->Activate();

    UAbilityTask_WaitGameplayEvent *GrabEnd =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_GrabEnd);
    GrabEnd->EventReceived.AddDynamic(this, &ThisClass::OnGrabEnd);
    GrabEnd->Activate();
}

void UGA_Grab::OnFollowUpInputReceived(FGameplayEventData EventData)
{
    FGameplayEventData SlamEventData;
    SlamEventData.Target = GrabbedEnemy.Get();
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetAvatarActorFromActorInfo(), Nyota::Event_Ability_HasGrabbedEnemy, SlamEventData
    );
}

void UGA_Grab::OnGrabEnd(FGameplayEventData EventData)
{
    OnGrabTimeout();
}

void UGA_Grab::OnMontageCompleted()
{
    UWorld *World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    AActor *AvatarActor = GetAvatarActorFromActorInfo();

    TArray<AActor *> HitActors = UBlueprintUtilsLibrary::HitBoxOverlapTest(
        World, AvatarActor, HitBoxRadius, AvatarActor->GetActorForwardVector(), 0.f, bDrawDebugs
    );

    // 没有抓到敌人则结束该技能
    if (HitActors.IsEmpty())
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

        return;
    }

    AActor *HitActor = HitActors[0];
    GrabbedEnemy = Cast<ABaseEnemyWildBoar>(HitActor);
    ABasePlayer *BasePlayer = Cast<ABasePlayer>(AvatarActor);
    if (!GrabbedEnemy.IsValid() || !IsValid(BasePlayer))
    {
        return;
    }

    // 敌人 Attach 到玩家抓取位置 Socket
    GrabbedEnemy->OnGrabbed(BasePlayer);

    // 抓取倒计时
    World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::OnGrabTimeout, 5.f, false);
}

void UGA_Grab::OnGrabTimeout()
{
    UWorld *World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    GrabbedEnemy->OnThrown(GetAvatarActorFromActorInfo()->GetActorForwardVector(), ThrownForce);

    GrabbedEnemy.Reset();

    World->GetTimerManager().ClearTimer(TimerHandle);

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}