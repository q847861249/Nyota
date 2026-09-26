// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "NyotaStatusComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * @brief 角色受控状态组件：把"状态 Tag → 行为冻结"的映射从角色类里剥离出来。
 * @details 监听宿主 ASC 上的屏蔽维度 Tag（Blocked.Movement / Blocked.AI），
 *          Tag 计数 >0 时执行冻结反应（锁移动模式、停 AI Tick），归零时自动恢复。
 *          状态的施加与到期完全交给限时 GameplayEffect 管理，本组件不持有计时器；
 *          新增眩晕/定身/沉默等状态 = 新建 GE 资产组合这些维度 Tag，无需修改角色类。
 *          由 ABaseCharacter 构造时默认创建，玩家与敌人共用同一套反应逻辑。
 */
UCLASS(ClassGroup = (Nyota), meta = (BlueprintSpawnableComponent))
class NYOTA_API UNyotaStatusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /**
     * @brief 向目标角色施加一个控制类状态 GE（统一施加入口）。
     * @details 内部先检查 Status_Immune_Control 免疫 Tag（如冲锋中的野猪），
     *          再通过限时 GE 授予屏蔽维度 Tag，生命周期由 GAS 管理。
     * @param Target       被施加状态的角色（需实现 IAbilitySystemInterface 且为 ABaseCharacter）。
     * @param StatusEffect 状态 GE 类：DurationPolicy 必须为 HasDuration，GrantedTags 填屏蔽维度 Tag。
     * @param Duration     >0 时覆盖 GE 资产里的持续时间；<=0 时使用资产自身配置。
     * @return true 表示已成功施加；false 表示目标无效、已死亡、免疫或资产未配置。
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | Status", meta = (DefaultToSelf = "Target"))
    static bool ApplyStatusEffect(AActor *Target, TSubclassOf<UGameplayEffect> StatusEffect, float Duration = 0.f);

protected:
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    /** ASC 延迟就绪回调（角色 BeginPlay 广播 OnASCInitialized 时补注册监听）。 */
    UFUNCTION()
    void OnASCInitialized(UAbilitySystemComponent *ASC, UAttributeSet *AS);

    /** 对当前 ASC 注册屏蔽维度 Tag 的计数监听。 */
    void RegisterBlockedTagEvents();

    /** Blocked.Movement 计数变化：>0 冻结移动模式，==0 恢复冻结前记录的模式。 */
    void OnBlockedMovementChanged(const FGameplayTag Tag, int32 NewCount);

    /** Blocked.AI 计数变化：>0 停 AIController Tick（行为树冻结），==0 恢复。 */
    void OnBlockedAIChanged(const FGameplayTag Tag, int32 NewCount);

    /** Status.Slowed 计数变化：>0 按 SlowSpeedMultiplier 降低移动速度，==0 恢复。与移动冻结正交（一个锁模式、一个改速度）。 */
    void OnSlowedChanged(const FGameplayTag Tag, int32 NewCount);

    /**
     * @brief 减速后保留的速度比例，取值 0~1：0.5 表示移速减半。
     * @details 在角色蓝图上可按角色配置不同的减速抗性（如野猪对减速不敏感可配高一些）。
     */
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Status", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float SlowSpeedMultiplier = 0.5f;

    /** 当前监听的 ASC（Owner 的 ASC，可能来自 PlayerState 或角色自身）。 */
    UPROPERTY(Transient)
    TObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;

    /** Blocked.Movement 的 Tag 事件句柄，EndPlay 时注销。 */
    FDelegateHandle MovementTagEventHandle;

    /** Blocked.AI 的 Tag 事件句柄，EndPlay 时注销。 */
    FDelegateHandle AITagEventHandle;

    /** Status.Slowed 的 Tag 事件句柄，EndPlay 时注销。 */
    FDelegateHandle SlowedTagEventHandle;

    /** 减速前的地面最大速度，解除时恢复。 */
    float SavedMaxWalkSpeed = 0.f;

    /** 是否已处于减速状态（防重复保存/重复恢复）。 */
    bool bSlowed = false;

    /** 冻结前的移动模式，解除时恢复，避免把 Falling 等状态错误恢复成 Walking。 */
    TEnumAsByte<EMovementMode> SavedMovementMode = MOVE_Walking;

    /** 是否已因 Blocked.Movement 冻结移动（防重复保存/重复恢复）。 */
    bool bMovementFrozen = false;

    /** 是否已因 Blocked.AI 停掉控制器 Tick。 */
    bool bAIFrozen = false;
};
