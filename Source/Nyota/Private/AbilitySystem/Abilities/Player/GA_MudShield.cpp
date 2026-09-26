// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_MudShield.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Character/BaseCharacter.h"
#include "GameplayEffect.h"
#include "GameplayTags/GameTags.h"

UGA_MudShield::UGA_MudShield(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    // 供蓝图/查找使用的能力标识
    AbilityTags.AddTag(Nyota::Ability_MudShield);

    // 可用性表：冲锋/推土机中不可释放（冲锋中普攻与技能二均不可用，撞人后走派生普攻）
    ActivationBlockedTags.AddTag(Nyota::Ability_State_Charging);
}

void UGA_MudShield::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ABaseCharacter *Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    UBaseAttributeSet *AttributeSet =
        IsValid(Character) ? Cast<UBaseAttributeSet>(Character->GetAttributeSet()) : nullptr;

    // 护盾结算依赖属性集与增益 GE 资产，缺失时直接失败结束
    if (!IsValid(AttributeSet) || !IsValid(ShieldBuffEffect))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

        return;
    }

    // 提交技能：触发消耗与冷却（冷却时长走基类 SetByCaller 冷却 GE）
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

        return;
    }

    // 重置结算/结束标记，本次激活的数值结算与技能收尾都交给蓝图蒙太奇驱动
    bShieldApplied = false;
    bFinishCalled = false;

    OnCastShield_Visual();

    // 激活后不自动结束：技能保持激活，等待蓝图在蒙太奇通知帧调用 ApplyShieldEffect()，
    // 并在蒙太奇播完/被打断时调用 FinishShieldCast() 收尾
}

void UGA_MudShield::ApplyShieldEffect()
{
    // 同一次激活只结算一次：蒙太奇通知可能被多次进入（如回跳帧），重复调用直接忽略
    if (bShieldApplied || !IsValid(ShieldBuffEffect))
    {
        return;
    }

    bShieldApplied = true;

    ABaseCharacter *Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    UBaseAttributeSet *AttributeSet =
        IsValid(Character) ? Cast<UBaseAttributeSet>(Character->GetAttributeSet()) : nullptr;

    if (!IsValid(AttributeSet))
    {
        return;
    }

    // 叠加护盾：直接写基础值（与 PreGameplayEffectExecute 的护盾消耗逻辑一致，避免聚合器污染）；
    // 重复释放时刷新为满盾
    AttributeSet->SetShield(ShieldAmount);

    // 施加限时增益：授予 Status.Shielded 标记 + 减伤加成，GE 到期自动还原减伤
    FGameplayEffectSpecHandle BuffSpecHandle = MakeOutgoingGameplayEffectSpec(ShieldBuffEffect, GetAbilityLevel());
    BuffSpecHandle.Data->SetSetByCallerMagnitude(Nyota::Data_DamageReduction, DamageReductionAmount);
    ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffSpecHandle);
}

void UGA_MudShield::FinishShieldCast(bool bWasCancelled)
{
    // 防止蓝图"播完"与"被打断"两条回调路径都触发导致二次 EndAbility
    if (bFinishCalled)
    {
        return;
    }

    bFinishCalled = true;

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bWasCancelled);
}
