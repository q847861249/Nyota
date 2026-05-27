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

TArray<AActor *> UGA_LightAttack::HitBoxOverlapTest()
{

    // ==============================
    // 1. 设置忽略 Actor（通常忽略自己）
    // ==============================

    TArray<AActor *> ActorsToIgnore;

    // 忽略当前技能拥有者（否则会打到自己）
    ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

    // 创建查询参数
    FCollisionQueryParams QueryParams;

    // 将忽略列表加入查询参数
    QueryParams.AddIgnoredActors(ActorsToIgnore);

    // ==============================
    // 2. 设置碰撞响应规则
    // ==============================

    FCollisionResponseParams ResponseParams;

    // 默认忽略所有碰撞通道
    ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);

    // 只检测 Pawn（角色）
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

    // ==============================
    // 3. 创建球形检测区域（Sphere HitBox）
    // ==============================

    // 根据半径创建球形碰撞体
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(HitBoxRadius);

    // ==============================
    // 4. 计算 HitBox 位置（角色前方）
    // ==============================

    // 获取角色前方向，并乘以前移距离，如果未设置使用默认获取到的值
    const FVector Forward = GetAbilityDetectionDirection() * HitBoxForwardOffset;

    // 最终 HitBox 位置：
    // 角色位置 + 前移 + 高度偏移
    const FVector ElevationOffset = FVector(0.0f, 0.0f, HitBoxElevationOffset);
    const FVector HitBoxLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + ElevationOffset;

    // ==============================
    // 5. 执行 Overlap 检测
    // ==============================

    // 用于存储检测到的结果
    TArray<FOverlapResult> OverlapResults;

    // 执行多目标重叠检测（Sphere Overlap）
    GetWorld()->OverlapMultiByChannel(
        OverlapResults,  // 输出：命中的结果列表
        HitBoxLocation,  // 检测位置
        FQuat::Identity, // 旋转（Sphere 不需要旋转）
        ECC_Visibility,  // 使用的检测通道
        CollisionShape,  // 碰撞体形状（Sphere）
        QueryParams,     // 查询参数（忽略 Actor）
        ResponseParams   // 碰撞响应规则
    );

    TArray<AActor *> HitActors;

    for (const FOverlapResult &Result : OverlapResults)
    {
        if (!IsValid(Result.GetActor()))
        {
            continue;
        }

        HitActors.AddUnique(Result.GetActor());
    }

    // ==============================
    // 6. Debug 绘制（可选）
    // ==============================

    if (bDrawDebugs)
    {
        DrawDebugInformation(OverlapResults, HitBoxLocation);
    }

    return HitActors;
}

void UGA_LightAttack::SendHitReactEventToActor(const TArray<AActor *> &HitActors)
{
    for (AActor *HitActor : HitActors)
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
        GetWorld(), Player, PrevLeftHandLocation, CurrentLeftHandLocation, TraceRadius, HitActors, bDrawDebugs
    );

    //--------------------------------
    // 右手检测
    //--------------------------------

    TArray<FHitResult> RightHandHitResults = UBlueprintUtilsLibrary::SocketSweepTest(
        GetWorld(), Player, PrevRightHandLocation, CurrentRightHandLocation, TraceRadius, HitActors, bDrawDebugs
    );

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