// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_Grab.h"

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
    PlayMontageTask->ReadyForActivation();

    UAbilityTask_WaitGameplayEvent *StartGrabTrace =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_StartGrabTrace);
    StartGrabTrace->EventReceived.AddDynamic(this, &ThisClass::OnStartGrabTrace);
    StartGrabTrace->ReadyForActivation();

    UAbilityTask_WaitGameplayEvent *StopGrabTrace =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_StopGrabTrace);
    StopGrabTrace->EventReceived.AddDynamic(this, &ThisClass::OnStopGrabTrace);
    StopGrabTrace->ReadyForActivation();

    UAbilityTask_WaitGameplayEvent *GrabEnd =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_GrabEnd);
    GrabEnd->EventReceived.AddDynamic(this, &ThisClass::OnGrabEnd);
    GrabEnd->ReadyForActivation();
}

void UGA_Grab::OnMontageCompleted()
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        return;
    }

    // 没有抓到敌人则结束该技能
    if (!IsValid(Player->GetGrabbedEnemy()))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UGA_Grab::OnGrabEnd(FGameplayEventData EventData)
{
    OnGrabTimeout();
}

void UGA_Grab::OnGrabTimeout()
{
    UWorld *World = GetWorld();

    if (!IsValid(World))
    {
        return;
    }

    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        return;
    }

    ABaseEnemyWildBoar *WildBoar = Cast<ABaseEnemyWildBoar>(Player->GetGrabbedEnemy());

    if (!IsValid(WildBoar))
    {
        return;
    }

    WildBoar->OnThrown(GetAvatarActorFromActorInfo()->GetActorForwardVector(), ThrownForce);

    Player->ResetGrabbedEnemy();

    World->GetTimerManager().ClearTimer(TimerHandle);

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Grab::OnStartGrabTrace(FGameplayEventData EventData)
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        return;
    }

    USkeletalMeshComponent *Mesh = Player->GetMesh();

    if (!IsValid(Mesh))
    {
        return;
    }

    if (!IsValid(GetWorld()))
    {
        return;
    }

    //--------------------------------
    // 初始化上一帧位置
    //--------------------------------

    PrevLeftHandLocation = Mesh->GetSocketLocation(LeftHandSocketName);
    PrevRightHandLocation = Mesh->GetSocketLocation(RightHandSocketName);

    //--------------------------------
    // 开启 Tick 检测
    //--------------------------------

    GetWorld()->GetTimerManager().SetTimer(
        GrabTraceTimerHandle, this, &ThisClass::PerformGrabTrace, GrabTraceRate, true
    );
}

void UGA_Grab::OnStopGrabTrace(FGameplayEventData EventData)
{
    if (!IsValid(GetWorld()))
    {
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(GrabTraceTimerHandle);
}

void UGA_Grab::PerformGrabTrace()
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());

    if (!IsValid(Player))
    {
        return;
    }

    // 如果有抓取到的敌人则不执行检测
    if (IsValid(Player->GetGrabbedEnemy()))
    {
        return;
    }

    USkeletalMeshComponent *Mesh = Player->GetMesh();

    if (!IsValid(Mesh))
    {
        return;
    }

    if (!IsValid(GetWorld()))
    {
        return;
    }

    //--------------------------------
    // 当前手位置
    //--------------------------------

    FVector CurrentLeftHandLocation = Mesh->GetSocketLocation(LeftHandSocketName);
    FVector CurrentRightHandLocation = Mesh->GetSocketLocation(RightHandSocketName);

    //--------------------------------
    // 左手检测
    //--------------------------------

    TArray<FHitResult> LeftHandHitResults = UBlueprintUtilsLibrary::SocketSweepTest(
        GetWorld(), Player, PrevLeftHandLocation, CurrentLeftHandLocation, GrabTraceRadius, {Player}, bDrawDebugs
    );

    //--------------------------------
    // 右手检测
    //--------------------------------

    TArray<FHitResult> RightHandHitResults = UBlueprintUtilsLibrary::SocketSweepTest(
        GetWorld(), Player, PrevRightHandLocation, CurrentRightHandLocation, GrabTraceRadius, {Player}, bDrawDebugs
    );

    // 没有抓到敌人
    if (LeftHandHitResults.IsEmpty() && RightHandHitResults.IsEmpty())
    {
        //--------------------------------
        // 更新上一帧
        //--------------------------------

        PrevLeftHandLocation = CurrentLeftHandLocation;
        PrevRightHandLocation = CurrentRightHandLocation;

        return;
    }

    AActor *HitActor = nullptr;

    if (!LeftHandHitResults.IsEmpty())
    {
        HitActor = LeftHandHitResults[0].GetActor();
    }
    else
    {
        HitActor = RightHandHitResults[0].GetActor();
    }

    ABaseEnemyWildBoar *GrabbedEnemy = Cast<ABaseEnemyWildBoar>(HitActor);

    if (IsValid(GrabbedEnemy))
    {
        // 抓取到敌人缓存到 Player 中
        Player->SetGrabbedEnemy(GrabbedEnemy);

        // 敌人 Attach 到玩家抓取位置 Socket
        GrabbedEnemy->OnGrabbed(Player);

        // 抓取倒计时
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::OnGrabTimeout, GrabTimerRate, false);

        // 停止检测
        OnStopGrabTrace({});
    }

    //--------------------------------
    // 更新上一帧
    //--------------------------------

    PrevLeftHandLocation = CurrentLeftHandLocation;
    PrevRightHandLocation = CurrentRightHandLocation;
}