// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_MudShield.generated.h"

class UGameplayEffect;

/**
 * @brief 泥土护盾（技能二）：防御 + 护盾。
 * @details 野猪用泥土包裹自身：给 Shield 属性写入护盾值，同时施加限时增益 GE
 *          （授予 Status.Shielded 标记 + DamageReduction 减伤加成，到期自动还原）。
 *          伤害结算由 UBaseAttributeSet::PreGameplayEffectExecute 统一拦截：
 *          先按 DamageReduction 比例减伤，再优先由护盾吸收，剩余伤害才落到 Health。
 *          护盾值不随时间衰减，只随受击消耗；重复释放会刷新护盾值与增益时长。
 *          可用性（对应设计表）：冲锋/推土机中不可激活。
 *          流程（由蓝图蒙太奇驱动，激活后不自动结束）：
 *          激活时提交消耗/冷却并调用 OnCastShield_Visual 播放蒙太奇；
 *          蓝图在蒙太奇的通知帧调用 ApplyShieldEffect() 完成护盾与减伤结算；
 *          蒙太奇播完（或被打断）后调用 FinishShieldCast() 结束技能。
 */
UCLASS()
class NYOTA_API UGA_MudShield : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_MudShield(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    /**
     * @brief 应用护盾结算：写入 Shield 属性 + 施加限时减伤增益 GE。
     * @details 由蓝图在施放蒙太奇的通知帧手动调用，让数值结算与表现节奏对齐；
     *          同一次激活内只结算一次，重复调用会被忽略。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | MudShield")
    void ApplyShieldEffect();

    /**
     * @brief 结束泥土护盾技能（激活后不会自动结束，由蓝图在蒙太奇结束时调用）。
     * @param bWasCancelled true 表示蒙太奇被打断（按取消结束）；false 表示正常播完结束。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | MudShield")
    void FinishShieldCast(bool bWasCancelled = false);

protected:
    /** 护盾吸收量，单位：点。直接写入 Shield 属性基础值，随受击消耗，不随时间衰减。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | MudShield")
    float ShieldAmount = 50.f;

    /**
     * @brief 防御力提升比例（减伤比例），范围 0~1。
     * @details 通过 Data.DamageReduction SetByCaller 写入增益 GE 的 Additive 修饰符，
     *          GE 到期时自动还原为 0，无需手动清理。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | MudShield", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DamageReductionAmount = 0.3f;

    /**
     * @brief 护盾增益 GE：限时授予 Status.Shielded 标记，并对 DamageReduction 属性做 Additive 修饰。
     * @details GE 的 Duration 决定防御力提升的持续时间；修饰量由 DamageReductionAmount 经 SetByCaller 传入。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | MudShield")
    TSubclassOf<UGameplayEffect> ShieldBuffEffect;

    /** 蓝图播放施放泥土护盾表现（裹泥动画、尘土特效等），激活时触发一次。 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Nyota | MudShield")
    void OnCastShield_Visual();

private:
    /** 本次激活是否已完成护盾结算，防止蓝图蒙太奇通知重复调用 ApplyShieldEffect 重复叠盾。 */
    bool bShieldApplied = false;

    /** 本次激活是否已请求结束，防止 FinishShieldCast 被蓝图多条路径重复触发导致二次 EndAbility。 */
    bool bFinishCalled = false;
};
