// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_BoarSlam.generated.h"

class ABaseCharacter;
class UGameplayEffect;

/**
 * @brief 野猪砸地（技能三）：AOE + 减速/击退。
 * @details 以野猪为中心对圆形范围内的敌人造成 AOE 效果，全程蓝图蒙太奇驱动：
 *          - 普通释放：原地砸地无位移，触发 OnSlamLand_Visual 播放蒙太奇；
 *            砸地通知帧调 ApplySlamAoe() 结算范围减速（可选伤害 GE），蒙太奇结束调 FinishSlam() 收尾；
 *          - 冲锋中派生（未撞人时按技能三）：激活时通过 CancelAbilitiesWithTag 自动取消冲锋技能，
 *            并由 C++ 发起 JumpForce RootMotionSource 原地跳跃（替代 LaunchCharacter），
 *            触发 OnJump_Visual 播放跳跃砸地蒙太奇；砸地帧调 ApplySlamAoe() 结算小范围击退
 *            （同样用 JumpForce 把敌人沿中心向外抛物线击飞），蒙太奇结束调 FinishSlam() 收尾。
 *          可用性（对应设计表）：推土机状态不可激活（已撞人）；释放期间通过 ActivationOwnedTags
 *          挂 Ability_InputBlocked，屏蔽所有其他技能输入（"技能三释放中"全列不可用）。
 *          C++ 只负责派生判定、跳跃位移与 AOE 数值结算，表现节奏全部交给蓝图蒙太奇。
 */
UCLASS()
class NYOTA_API UGA_BoarSlam : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_BoarSlam(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    /**
     * @brief 激活前判定派生模式：冲锋中激活 → 本技能将取消冲锋并派生跳跃砸地。
     * @details 必须在 Super::PreActivate 之前检测 Charging 标记：Super 会应用
     *          CancelAbilitiesWithTag 取消冲锋技能，而 GA_Charge::EndAbility 会立刻
     *          移除 Ability_State_Charging，届时再检测将永远为 false。
     */
    virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate *OnGameplayAbilityEndedDelegate, const FGameplayEventData *TriggerEventData = nullptr) override;

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
    ) override;

    /** 蓝图播放跳跃砸地蒙太奇（仅冲锋派生时触发；跳跃位移已由 C++ 的 RootMotionSource 发起）。 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Nyota | BoarSlam")
    void OnJump_Visual();

    /** 蓝图播放原地砸地蒙太奇（普通释放时触发；无跳跃位移）。 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Nyota | BoarSlam")
    void OnSlamLand_Visual();

    /**
     * @brief 执行砸地 AOE 结算：普通模式 = 范围减速（+可选伤害），冲锋派生 = 小范围击退。
     * @details 由蓝图在砸地蒙太奇的通知帧手动调用，让数值结算与表现节奏对齐；
     *          同一次激活内只结算一次，重复调用会被忽略。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | BoarSlam")
    void ApplySlamAoe();

    /**
     * @brief 结束砸地技能（激活后不会自动结束，由蓝图在蒙太奇结束时调用）。
     * @param bWasCancelled true 表示蒙太奇被打断（按取消结束）；false 表示正常播完结束。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | BoarSlam")
    void FinishSlam(bool bWasCancelled = false);

protected:
    /** 普通砸地的 AOE 半径，单位：cm。以野猪为中心的圆形区域。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float SlamRadius = 300.f;

    /** 冲锋派生砸地的击退 AOE 半径，单位：cm（设计为"小范围"，可小于普通半径）。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float DerivedKnockbackRadius = 200.f;

    /** 派生跳跃的抛物线顶点高度（原地起跳落回原位），单位：cm。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float JumpHeight = 400.f;

    /** 派生跳跃的总时长（起跳到落回地面），单位：秒。蒙太奇节奏需与之对齐。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float JumpDuration = 1.f;

    /** 派生击退的水平总距离，单位：cm。方向为敌人相对野猪的向外方向。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float KnockbackDistance = 400.f;

    /** 派生击退的抛物线顶点高度（上抛高度），单位：cm。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float KnockbackHeight = 150.f;

    /** 派生击退的总时长（击飞到落地），单位：秒。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float KnockbackDuration = 0.5f;

    /** 普通砸地的减速时长，单位：秒。覆盖减速 GE 资产里的时长。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    float SlowDuration = 2.f;

    /**
     * @brief 减速状态 GE：限时授予 Status.Slowed Tag。
     * @details 由 UNyotaStatusComponent 监听该 Tag 并按 SlowSpeedMultiplier 降低移动速度，到期自动恢复；
     *          与眩晕 GE 一样可多技能共用资产，时长由 SlowDuration 覆盖。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    TSubclassOf<UGameplayEffect> SlowStatusEffect;

    /**
     * @brief 普通砸地附带的可选伤害 GE（如需要"伤害+减速"而非纯减速时配置）。
     * @details 伤害量在 GE 资产里配置；留空表示砸地只减速不掉血。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | BoarSlam")
    TSubclassOf<UGameplayEffect> DamageEffect;

private:
    /** @return 本次结算半径：派生模式用 DerivedKnockbackRadius，普通模式用 SlamRadius。 */
    float GetSlamRadius() const;

    /** 本次激活是否已完成 AOE 结算，防止蓝图蒙太奇通知重复调用 ApplySlamAoe 重复结算。 */
    bool bSlamApplied = false;

    /** 本次激活是否已请求结束，防止 FinishSlam 被蓝图多条路径重复触发导致二次 EndAbility。 */
    bool bFinishCalled = false;

    /** 是否为冲锋派生模式（PreActivate 时检测 Ability_State_Charging 决定，Super 之前判定）。 */
    bool bDerivedFromCharge = false;
};
