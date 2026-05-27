// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/Player/GA_LightAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"
#include "AbilitySystemComponent.h"
#include "Utils/BlueprintUtilsLibrary.h"

void UGA_LightAttack::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilityTask_PlayMontageAndWait *Task =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("AttackMontage"), AttackMontage);
    Task->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
    Task->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageCompleted);
    Task->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCompleted);
    Task->ReadyForActivation();

    // AttackStart
    UAbilityTask_WaitGameplayEvent *AttackStart =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_AttackStart);
    AttackStart->EventReceived.AddDynamic(this, &ThisClass::OnStartLightAttackTrace);
    AttackStart->ReadyForActivation();

    // AttackEnd
    UAbilityTask_WaitGameplayEvent *AttackEnd =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_AttackEnd);
    AttackEnd->EventReceived.AddDynamic(this, &ThisClass::OnAttackEnd);
    AttackEnd->ReadyForActivation();
}

void UGA_LightAttack::SendHitReactEventToActor(const TArray<AActor *> &HitActorList)
{
    for (AActor *HitActor : HitActorList)
    {
        FGameplayEventData PayloadData;
        PayloadData.Instigator = GetAvatarActorFromActorInfo();

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, Nyota::Event_Enemy_HitReact, PayloadData);
    }
}

void UGA_LightAttack::SetLookAtEnemyRotation(AActor *LookAtActor) const
{
    // 设置玩家到敌人的旋转
    if (IsValid(LookAtActor))
    {
        // 计算玩家到敌人的旋转角度
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
            GetAvatarActorFromActorInfo()->GetActorLocation(), LookAtActor->GetActorLocation()
        );

        // 这里如果是螃蟹，需要调整 Yaw
        ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
        if (IsValid(Player) && Player->GetPlayerType() == EPlayerType::Crab)
        {
            LookAtRotation.Yaw -= 90;
        }

        // 设置玩家面朝敌人
        GetAvatarActorFromActorInfo()->SetActorRotation(LookAtRotation);
    }
}

void UGA_LightAttack::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_LightAttack::ApplyDamage(const TArray<AActor *> &DamageActors)
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();
    if (!IsValid(ASC))
    {
        return;
    }

    SendHitReactEventToActor(DamageActors);

    for (AActor *HitActor : DamageActors)
    {
        ABaseCharacter *BaseCharacter = Cast<ABaseCharacter>(HitActor);
        if (!IsValid(BaseCharacter))
        {
            continue;
        }

        FGameplayEffectSpecHandle SpecHandle =
            ASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), FGameplayEffectContextHandle());

        ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), BaseCharacter->GetAbilitySystemComponent());
    }
}

void UGA_LightAttack::OnAttackEnd(FGameplayEventData EventData)
{
    OnStopLightAttackTrace();

    ApplyDamage(HitActors);

    // Clear HitActors
    HitActors.Empty();
}

void UGA_LightAttack::OnStartLightAttackTrace(FGameplayEventData EventData)
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
        TraceTimerHandle, this, &ThisClass::PerformLightAttackTrace, TraceRate, true
    );
}

void UGA_LightAttack::OnStopLightAttackTrace()
{
    if (!IsValid(GetWorld()))
    {
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
}

void UGA_LightAttack::PerformLightAttackTrace()
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

    UWorld *World = GetWorld();
    if (!IsValid(World))
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

    TArray<FHitResult> LeftHandHitResults = {};
    if (!LeftHandSocketName.IsNone())
    {
        LeftHandHitResults = UBlueprintUtilsLibrary::SocketSweepTest(
            World, Player, PrevLeftHandLocation, CurrentLeftHandLocation, TraceRadius, HitActors, bDrawDebugs
        );
    }

    //--------------------------------
    // 右手检测
    //--------------------------------

    TArray<FHitResult> RightHandHitResults = {};
    if (!RightHandSocketName.IsNone())
    {
        RightHandHitResults = UBlueprintUtilsLibrary::SocketSweepTest(
            World, Player, PrevRightHandLocation, CurrentRightHandLocation, TraceRadius, HitActors, bDrawDebugs
        );
    }

    // 没有碰到任何敌人
    if (LeftHandHitResults.IsEmpty() && RightHandHitResults.IsEmpty())
    {
        //--------------------------------
        // 更新上一帧
        //--------------------------------

        PrevLeftHandLocation = CurrentLeftHandLocation;
        PrevRightHandLocation = CurrentRightHandLocation;

        return;
    }

    for (const FHitResult &Result : LeftHandHitResults)
    {
        if (!IsValid(Result.GetActor()))
        {
            continue;
        }

        HitActors.Add(Result.GetActor());
    }

    for (const FHitResult &Result : RightHandHitResults)
    {
        if (!IsValid(Result.GetActor()))
        {
            continue;
        }

        HitActors.Add(Result.GetActor());
    }

    //--------------------------------
    // 更新上一帧
    //--------------------------------

    PrevLeftHandLocation = CurrentLeftHandLocation;
    PrevRightHandLocation = CurrentRightHandLocation;
}

void UGA_LightAttack::DrawDebugInformation(
    const TArray<FOverlapResult> &OverlapResults, const FVector &HitBoxLocation
) const
{
    // 绘制检测球体（红色）
    DrawDebugSphere(GetWorld(), HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

    // 绘制命中的 Actor 位置
    for (const FOverlapResult &Result : OverlapResults)
    {
        if (!IsValid(Result.GetActor()))
        {
            continue;
        }

        // 获取命中 Actor 位置
        FVector ActorLocation = Result.GetActor()->GetActorLocation();

        // 在 Actor 位置画球
        DrawDebugSphere(GetWorld(), ActorLocation, 30.f, 10, FColor::Green, false, 3.f);
    }
}