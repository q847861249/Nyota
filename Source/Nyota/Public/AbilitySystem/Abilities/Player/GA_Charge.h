// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_Charge.generated.h"

class ABaseCharacter;
class UAbilitySystemComponent;
class UAbilityTask_EveryFrameTick;
class UGameplayEffect;

/**
 * @brief 野猪冲锋技能（技能一）：位移 + 控制。
 * @details 激活后野猪沿当前朝向高速前冲，期间免疫所有控制效果并降低转向速度（难以急转弯）。
 *          撞到敌人后进入"推土机状态"：把敌人固定在身前持续向前推动，敌人无法移动和释放技能；
 *          推动直到撞墙结束（撞墙时敌人被眩晕）或玩家主动派生普攻把敌人顶飞。
 *          空冲撞墙（没有推到人）则野猪自身被眩晕作为惩罚。
 *          派生窗口：推土机状态下可激活派生普攻 GA_ChargeLightAttack（顶飞敌人并结束冲锋）。
 *          本类负责 C++ 侧的移动/碰撞/推人逻辑，动画与特效通过蓝图事件（*Visual）在子类中表现。
 */
UCLASS()
class NYOTA_API UGA_Charge : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_Charge(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
    ) override;

    /**
     * @brief 启动冲锋：挂冲锋状态 Tag、提高冲刺速度、开启前冲与碰撞检测 Tick。
     * @details 转向降速（难以急转弯）由 UNyotaHeroComponent 检测 Ability_State_Charging 后切换插值速度实现。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | Charge")
    void StartCharging();

    /**
     * @brief 主动结束冲锋（派生普攻顶飞敌人后由 GA_ChargeLightAttack 调用）。
     * @details 负责恢复移动设置、清理推土机状态；不会触发撞墙眩晕惩罚。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | Charge")
    void FinishCharge();

    /** 当前被推土机状态固定的敌人；为空表示不在推人状态。 */
    UFUNCTION(BlueprintPure, Category = "Nyota | Charge")
    ABaseCharacter *GetBulldozedEnemy() const
    {
        return BulldozedEnemy.Get();
    }

    /**
     * @brief 派生普攻把敌人顶飞后调用：解除推土机绑定并结束冲锋。
     * @param EnemyToLaunch 被顶飞的敌人。
     * @details 由 GA_ChargeLightAttack 通过 ASC 找到本技能实例调用，避免两个技能间直接持有引用。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | Charge")
    void LaunchBulldozedEnemy(ABaseCharacter *EnemyToLaunch);

protected:
    /** 冲锋冲刺速度，单位：cm/s。每帧通过 LaunchCharacter 覆写水平速度，保证与帧率无关。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float ChargeSpeed = 1200.f;

    /** 前向碰撞检测球的半径，单位：cm。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float HitRadius = 60.f;

    /** 推土机状态下敌人身位相对野猪胶囊中心的高度偏移，单位：cm。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float BulldozeZOffset = 0.f;

    /**
     * @brief 推土机状态下敌人固定在野猪前方的距离，单位：cm。
     * @details 过小时会在 EnterBulldozeState 中被钳制到"双方胶囊半径之和 + 安全间隙"，
     *          否则敌人会被每帧传进野猪胶囊内部，穿透解算互推导致野猪反向奔跑。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float BulldozeForwardOffset = 120.f;

    /**
     * @brief 判定为"墙/障碍"所需的最小法线夹角，单位：度。
     * @details 命中法线与竖直向上的夹角超过该值才算撞墙（墙体法线接近水平≈90°，地面≈0°），
     *          避免冲锋扫掠擦到地面/缓坡时误触发撞墙停下。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float WallMinNormalAngleFromUp = 45.f;

    /** 撞墙眩晕时长（野猪自身），单位：秒。作为 Duration 覆盖状态 GE 资产里的时长。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float SelfStunDuration = 1.5f;

    /** 推人撞墙时敌人的眩晕时长，单位：秒。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float EnemyStunDuration = 2.f;

    /**
     * @brief 眩晕状态 GE：限时授予 Blocked.Movement / Blocked.AI / Blocked.Ability 三个屏蔽维度 Tag。
     * @details 生命周期由 GAS 管理，UNyotaStatusComponent 负责冻结/恢复反应；
     *          资产本身 Duration 随意配，实际时长由上面的 Duration 属性覆盖，多个技能可共用同一资产。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    TSubclassOf<UGameplayEffect> StunStatusEffect;

    /** 冲锋最大持续时间，单位：秒。超时自动结束，防止无限冲锋。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float MaxChargeDuration = 6.f;

    /** 顶飞敌人的初速度，单位：cm/s。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float LaunchForce = 800.f;

    /** 顶飞敌人的上抛速度，单位：cm/s。 */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Charge")
    float LaunchZOffset = 350.f;

    /** 蓝图播放冲锋启动表现。 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Nyota | Charge")
    void OnChargeStart_Visual();

    /** 蓝图播放推土机撞到人表现。 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Nyota | Charge")
    void OnBulldozeStart_Visual();

    /** 蓝图播放撞墙眩晕表现。 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Nyota | Charge")
    void OnSmashWall_Visual();

private:
    /** 每帧由 UAbilityTask_EveryFrameTick 回调一次：覆写前冲速度、检测敌人/墙体、维持推土机状态。 */
    UFUNCTION()
    void OnChargeTick(float DeltaTime);

    /** 撞到敌人：进入推土机状态，把敌人绑定到身前。 */
    void EnterBulldozeState(ABaseCharacter *Enemy);

    /** 撞到障碍：区分推人撞墙（眩晕敌人）与空冲撞墙（眩晕自己）两种结算。 */
    void HandleWallImpact();

    /**
     * @brief 判断一次命中是否为"墙/障碍"而非地面等低角度表面。
     * @param Hit 扫掠命中结果（使用世界空间 ImpactNormal）。
     * @return true 表示法线与竖直向上的夹角大于 WallMinNormalAngleFromUp，应触发撞墙结算。
     */
    bool IsWallHit(const FHitResult &Hit) const;

    /** 解除推土机绑定并恢复敌人行动。 */
    void ReleaseBulldozedEnemy();

    /** 冲锋状态 Tag 的持有者（ASC），用于 EndAbility 时兜底清理。 */
    UPROPERTY(Transient)
    TObjectPtr<UAbilitySystemComponent> TaggedAbilitySystemComponent;

    /** 当前被推的敌人。 */
    TWeakObjectPtr<ABaseCharacter> BulldozedEnemy;

    /** 冲锋逐帧驱动任务（借道 ASC 的 TickingTasks 获得每帧回调）。 */
    UPROPERTY(Transient)
    TObjectPtr<UAbilityTask_EveryFrameTick> ChargeTickTask;

    /** 冲锋总时长计时器。 */
    FTimerHandle MaxDurationTimerHandle;

    /** 是否已经撞墙结算过，避免一次冲锋内重复触发。 */
    bool bWallImpacted = false;

    /** 是否已给野猪挂 Status_Immune_Control，防止 EndAbility 与撞墙自罚路径重复移除。 */
    bool bImmunityTagGranted = false;

    /** 是否已给被推敌人挂屏蔽维度 Tag，防止重复授予/移除。 */
    bool bEnemyBlockedTagsGranted = false;

    /** 冲锋是否已真正启动（移动参数已修改），EndAbility 据此决定是否需要恢复。 */
    bool bChargeStarted = false;

    /** 冲锋前记录的地面最大速度，EndAbility 时恢复。 */
    float SavedMaxWalkSpeed = 0.f;

    /** 实际使用的推人距离：EnterBulldozeState 时由 BulldozeForwardOffset 钳制而来（下限 = 双方胶囊半径之和 + 安全间隙），防止敌人被钉进野猪胶囊内。 */
    float EffectiveBulldozeForwardOffset = 0.f;
};
