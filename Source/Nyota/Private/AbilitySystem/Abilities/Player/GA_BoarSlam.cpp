// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_BoarSlam.h"

#include "AbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/NyotaStatusComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "GameplayEffect.h"
#include "GameplayTags/GameTags.h"

namespace
{
    /** 本技能发起的自跳 RootMotionSource 实例名；EndAbility 时按名移除，防止取消后残留位移。 */
    const FName JumpRootMotionName = TEXT("BoarSlam.Jump");

    /** 跳跃/击退 RootMotionSource 的优先级：取较高值，确保覆盖其他低优先级位移来源。 */
    constexpr uint16 RootMotionPriority = 1000;
}

UGA_BoarSlam::UGA_BoarSlam(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    AbilityTags.AddTag(Nyota::Ability_Slam);

    // 激活时自动取消冲锋技能（冲锋中未撞人按技能三 → 取消冲锋并派生跳跃砸地）；
    // 引擎在 PreActivate 里应用该取消（先于 ActivateAbility），派生判定也放在 PreActivate，见其注释
    CancelAbilitiesWithTag.AddTag(Nyota::Ability_Charge);

    // 推土机状态下不可激活（设计派生表：已撞人时按技能三无法派生）
    ActivationBlockedTags.AddTag(Nyota::Ability_State_Charging_Bulldozer);

    // 释放期间屏蔽所有技能输入（可用性表：技能三释放中 普攻/技能一/技能二/技能三 全列不可用）；
    // ActivationOwnedTags 在激活时挂到 ASC、结束时自动移除，ProcessAbilityInput 的总闸会因此清空输入
    ActivationOwnedTags.AddTag(Nyota::Ability_InputBlocked);
}

void UGA_BoarSlam::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate *OnGameplayAbilityEndedDelegate,
    const FGameplayEventData *TriggerEventData)
{
    // 必须在 Super::PreActivate 之前判定派生模式：Super 应用 CancelAbilitiesWithTag 会取消冲锋，
    // GA_Charge::EndAbility 随之移除 Ability_State_Charging，之后再检测将永远为 false
    bDerivedFromCharge = false;

    if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid() &&
        ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(Nyota::Ability_State_Charging))
    {
        bDerivedFromCharge = true;
    }
    
    Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
    
}



void UGA_BoarSlam::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

    if (!IsValid(Character) || !IsValid(GetWorld()))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

        return;
    }

    // 重置结算/结束标记，本次激活的 AOE 结算与技能收尾都交给蓝图蒙太奇驱动
    bSlamApplied = false;
    bFinishCalled = false;

    if (bDerivedFromCharge)
    {
        UCharacterMovementComponent *Movement = Character->GetCharacterMovement();

        if (!IsValid(Movement))
        {
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

            return;
        }

        // 派生模式：用 JumpForce RootMotionSource 原地跳跃（Distance=0 只走抛物线高度），
        // 替代 LaunchCharacter——位移全程由 RootMotion 驱动，与蒙太奇节奏对齐且随技能取消
        TSharedPtr<FRootMotionSource_JumpForce> JumpForce = MakeShared<FRootMotionSource_JumpForce>();
        JumpForce->InstanceName = JumpRootMotionName;
        JumpForce->AccumulateMode = ERootMotionAccumulateMode::Override;
        JumpForce->Priority = RootMotionPriority;
        JumpForce->Duration = JumpDuration;
        JumpForce->Distance = 0.f;
        JumpForce->Height = JumpHeight;
        JumpForce->Rotation = Character->GetActorRotation();
        JumpForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::SetVelocity;
        JumpForce->FinishVelocityParams.SetVelocity = FVector::ZeroVector;
        Movement->ApplyRootMotionSource(JumpForce);

        OnJump_Visual();

        return;
    }

    // 普通模式：原地砸地无位移，触发表现事件后保持激活，
    // 由蓝图在砸地通知帧调 ApplySlamAoe() 结算、蒙太奇结束调 FinishSlam() 收尾
    OnSlamLand_Visual();
}

void UGA_BoarSlam::EndAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
)
{
    // 技能结束时移除未跑完的跳跃 RootMotionSource（如蒙太奇被打断提前收尾），
    // 让角色立即回到物理移动状态；正常跑完时来源已自然过期，移除无副作用
    if (ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()); IsValid(Character))
    {
        if (UCharacterMovementComponent *Movement = Character->GetCharacterMovement(); IsValid(Movement))
        {
            Movement->RemoveRootMotionSource(JumpRootMotionName);
        }
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_BoarSlam::ApplySlamAoe()
{
    // 同一次激活只结算一次：蒙太奇通知可能被多次进入（如回跳帧），重复调用直接忽略
    if (bSlamApplied)
    {
        return;
    }

    bSlamApplied = true;

    ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    UWorld *World = IsValid(Character) ? Character->GetWorld() : nullptr;

    if (!IsValid(Character) || !IsValid(World))
    {
        return;
    }

    const FVector Center = Character->GetActorLocation();
    const float Radius = GetSlamRadius();

    if (bDrawDebugs)
    {
        DrawDebugSphere(World, Center, Radius, 24, bDerivedFromCharge ? FColor::Orange : FColor::Green, false, 1.5f);
    }

    // 以野猪为中心的圆形 AOE，抓取范围内所有存活敌人
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Character);

    TArray<FOverlapResult> Overlaps;
    World->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(Radius), QueryParams);

    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();

    for (const FOverlapResult &Overlap : Overlaps)
    {
        ABaseCharacter *Enemy = Cast<ABaseCharacter>(Overlap.GetActor());

        if (!IsValid(Enemy) || !Enemy->IsAlive())
        {
            continue;
        }

        if (bDerivedFromCharge)
        {
            // 派生结算：小范围击退——用 JumpForce 把敌人沿"中心向外"方向抛物线击飞
            //（水平 KnockbackDistance、上抛 KnockbackHeight、用时 KnockbackDuration），
            // 替代 LaunchCharacter，位移由 RootMotion 驱动，结束后速度清零防止滑行
            FVector Direction = Enemy->GetActorLocation() - Center;
            Direction.Z = 0.f;

            if (Direction.IsNearlyZero())
            {
                Direction = Character->GetActorForwardVector();
            }

            Direction.Normalize();

            if (ACharacter *EnemyCharacter = Cast<ACharacter>(Enemy);
                IsValid(EnemyCharacter) && IsValid(EnemyCharacter->GetCharacterMovement()))
            {
                TSharedPtr<FRootMotionSource_JumpForce> KnockbackSource = MakeShared<FRootMotionSource_JumpForce>();
                KnockbackSource->InstanceName = TEXT("BoarSlam.Knockback");
                KnockbackSource->AccumulateMode = ERootMotionAccumulateMode::Override;
                KnockbackSource->Priority = RootMotionPriority;
                KnockbackSource->Duration = KnockbackDuration;
                KnockbackSource->Distance = KnockbackDistance;
                KnockbackSource->Height = KnockbackHeight;
                // JumpForce 只取 Yaw：抛物线沿该朝向的水平前方击飞
                KnockbackSource->Rotation = FRotator(0.f, Direction.Rotation().Yaw, 0.f);
                KnockbackSource->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::SetVelocity;
                KnockbackSource->FinishVelocityParams.SetVelocity = FVector::ZeroVector;
                EnemyCharacter->GetCharacterMovement()->ApplyRootMotionSource(KnockbackSource);
            }
        }
        else
        {
            // 普通结算：AOE 减速（可被控制免疫拦截，如冲锋中的野猪），可选附带伤害
            UNyotaStatusComponent::ApplyStatusEffect(Enemy, SlowStatusEffect, SlowDuration);

            if (IsValid(ASC) && IsValid(DamageEffect))
            {
                FGameplayEffectSpecHandle SpecHandle =
                    ASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), FGameplayEffectContextHandle());
                ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Enemy->GetAbilitySystemComponent());
            }
        }
    }
}

void UGA_BoarSlam::FinishSlam(bool bWasCancelled)
{
    // 防止蓝图"播完"与"被打断"两条回调路径都触发导致二次 EndAbility
    if (bFinishCalled)
    {
        return;
    }

    bFinishCalled = true;

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bWasCancelled);
}

float UGA_BoarSlam::GetSlamRadius() const
{
    return bDerivedFromCharge ? DerivedKnockbackRadius : SlamRadius;
}
