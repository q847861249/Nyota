// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_PressureBlastWaterBall.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Actor/BaseProjectile.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseEnemyWildBoar.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"
void UGA_PressureBlastWaterBall::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // 提交技能，失败则直接结束技能
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    Execute();
}

void UGA_PressureBlastWaterBall::Execute()
{
    UAbilityTask_PlayMontageAndWait *Task =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PressureBlastWaterBall"), PressureBlastWaterBallMontage);
    Task->OnCompleted.AddDynamic(this, &ThisClass::Terminate);
    Task->OnInterrupted.AddDynamic(this, &ThisClass::Terminate);
    Task->OnCancelled.AddDynamic(this, &ThisClass::Terminate);
    Task->ReadyForActivation();

    UAbilityTask_WaitGameplayEvent *EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ShootWaterBall);
    EventTask->EventReceived.AddDynamic(this, &ThisClass::OnShootWaterBall);
    EventTask->ReadyForActivation();

    // 监听「结算伤害」AnimNotify 事件，对被喷射的敌人施加伤害
    UAbilityTask_WaitGameplayEvent *ApplyDamageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ApplyDamage);
    ApplyDamageTask->EventReceived.AddDynamic(this, &ThisClass::OnApplyDamage);
    ApplyDamageTask->ReadyForActivation();

    // 设定喷射力，供 GA_Grab / GA_PutDown 在抛出敌人时读取
    // 敌人与水球沿同一前方方向喷射，保证弧线一致
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (IsValid(Player))
    {
        if (IsValid(Player->GetGrabbedEnemy()))
        {
            Player->PendingThrownForce = PressureBlastForce;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UGA_PressureBlastWaterBall: No grabbed enemy to throw."));
        }
    }
}

void UGA_PressureBlastWaterBall::Terminate()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_PressureBlastWaterBall::OnShootWaterBall(FGameplayEventData EventData)
{
    SpawnWaterBall();
}

void UGA_PressureBlastWaterBall::SpawnWaterBall()
{
    ABaseCharacter *BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if (!BaseCharacter)
    {
        return;
    }

    UWorld *World = GetWorld();
    if (!World)
    {
        return;
    }

    // 获取开火位置（角色网格上指定插槽的世界坐标）作为水球的生成点
    FVector FireLocation = BaseCharacter->GetMesh()->GetSocketLocation(FireSocketName);
    // 发射方向使用角色前方（与敌人被喷射出的方向一致，保证二者弧线一致）
    FRotator FireRotation = BaseCharacter->GetActorForwardVector().Rotation();
    // 由旋转与位置构造生成变换，缩放保持为 1
    FTransform SpawnTransform(FireRotation, FireLocation, FVector(1.0f, 1.0f, 1.0f));

    // 配置生成参数
    FActorSpawnParameters Params;
    Params.Owner = BaseCharacter;
    Params.Instigator = BaseCharacter;
    // 始终生成，不进行碰撞检测，避免与角色自身碰撞导致生成失败
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    // 根组件缩放与传入的变换缩放相乘（此处为 1，即保持原始缩放）
    Params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

    // 生成水球投射物
    ABaseProjectile *WaterBallActor = World->SpawnActor<ABaseProjectile>(WaterBallClass, SpawnTransform, Params);
    if (!IsValid(WaterBallActor))
    {
        return;
    }

    // 让水球与敌人共享同一套初始速度 + 重力，保证二者弧线、落点、距离一致
    ABasePlayer *Player = Cast<ABasePlayer>(BaseCharacter);
    if (ABaseEnemyWildBoar *GrabbedEnemy = Cast<ABaseEnemyWildBoar>(Player ? Player->GetGrabbedEnemy() : nullptr))
    {
        const FVector LaunchVelocity = GrabbedEnemy->GetThrownLaunchVelocity(BaseCharacter->GetActorForwardVector(), PressureBlastForce);
        WaterBallActor->Launch(LaunchVelocity, 1.f);
    }
}

void UGA_PressureBlastWaterBall::OnApplyDamage(FGameplayEventData Payload)
{
    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!Player)
    {
        return;
    }

    ABaseCharacter *GrabbedEnemy = Player->GetGrabbedEnemy();
    if (!GrabbedEnemy)
    {
        return;
    }

    UAbilitySystemComponent *EnemyASC = GrabbedEnemy->GetAbilitySystemComponent();
    if (!EnemyASC)
    {
        return;
    }

    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        return;
    }

    // 对被喷射的敌人施加伤害效果，伤害量由 AnimNotify 传入的 EventMagnitude 决定
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PressureBlastEffect, Payload.EventMagnitude, FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), EnemyASC);
}
