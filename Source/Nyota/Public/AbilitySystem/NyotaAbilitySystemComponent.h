// Fill out your copyright notice in the Description page of Project Settings.
// NyotaAbilitySystemComponent - 项目的自定义 ASC
// 在 UE 原生 GAS 基础上扩展了基于 GameplayTag 的技能输入系统，
// 支持三种激活策略（按下触发、按住激活），并提供技能等级管理和 Tag 关系映射。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NyotaAbilitySystemComponent.generated.h"

class UNyotaAbilityTagRelationshipMapping;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYOTA_API UNyotaAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UNyotaAbilitySystemComponent(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    /**
     * @brief 技能被授予时回调
     *
     * 检查技能是否配置了自动激活 Tag（Ability_ActivateOnGive），
     * 用于实现被动技能、"获得即生效"等无需手动激活的技能。
     *
     * @param AbilitySpec 被授予的技能规格
     */
    virtual void OnGiveAbility(FGameplayAbilitySpec &AbilitySpec) override;

    /**
     * @brief 技能列表网络复制回调
     *
     * 客户端收到服务器同步的技能列表后，遍历所有可激活技能并尝试自动激活。
     * 使用 FScopedAbilityListLock 防止遍历期间列表被修改导致迭代器失效。
     */
    virtual void OnRep_ActivateAbilities() override;

    /**
     * @brief 初始化技能的 Owner/Avatar Actor 绑定
     *
     * 当新的 Pawn Avatar 被设置时，断言检查所有已注册技能都使用实例化策略
     *（NonInstanced 已废弃），确保输入事件和状态能够正确绑定到技能实例。
     *
     * @param InOwnerActor  技能的拥有者 Actor（通常是 PlayerState 或 Controller）
     * @param InAvatarActor 技能的化身 Actor（通常是 Pawn）
     */
    virtual void InitAbilityActorInfo(AActor *InOwnerActor, AActor *InAvatarActor) override;

    // -- 技能等级（仅服务端有效）--

    /**
     * @brief 设置技能等级为指定值
     *
     * 仅在服务端执行，客户端调用会被忽略。修改后标记 Spec 为脏触发网络同步。
     *
     * @param GameplayAbility 目标技能类
     * @param Level           要设置的等级值
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    void SetAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level);

    /**
     * @brief 增加技能等级
     *
     * 仅在服务端执行，客户端调用会被忽略。修改后标记 Spec 为脏触发网络同步。
     *
     * @param GameplayAbility 目标技能类
     * @param Level           增加的等级值（默认 +1）
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    void AddToAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level = 1);

    // -- 技能查询 --

    /**
     * @brief 通过 GameplayTag 精确匹配查找可激活的技能实例
     *
     * 遍历所有可激活技能，匹配其 AbilityTags 中是否包含指定的 Tag。
     *
     * @param AbilityTag 要匹配的 Tag
     * @return 匹配到的技能实例指针，未找到返回 nullptr
     */
    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    UGameplayAbility *GetActivatableAbilitySpecByTag(const FGameplayTag &AbilityTag);

    // -- 输入系统 --

    /**
     * @brief 输入 Tag 按下时的入口
     *
     * 遍历所有技能，将 DynamicAbilityTags 中包含该输入 Tag 的技能的 Handle
     * 加入按下缓存和按住缓存。通常由输入处理组件在按键按下时调用。
     *
     * @param InputTag 被按下的输入 Tag
     */
    void AbilityInputTagPressed(const FGameplayTag &InputTag);

    /**
     * @brief 输入 Tag 释放时的入口
     *
     * 遍历所有技能，将 DynamicAbilityTags 中包含该输入 Tag 的技能的 Handle
     * 加入释放缓存，同时从按住缓存中移除。通常由输入处理组件在按键释放时调用。
     *
     * @param InputTag 被释放的输入 Tag
     */
    void AbilityInputTagReleased(const FGameplayTag &InputTag);

    /**
     * @brief 每帧消费缓存的输入
     *
     * 核心输入处理管线，按以下顺序执行：
     * 1. 检查输入是否被阻塞（Ability_InputBlocked Tag）
     * 2. 处理按住输入 —— WhileInputActive 策略的未激活技能加入待激活队列
     * 3. 处理按下输入 —— 已激活技能转发事件，OnInputTriggered 策略的未激活技能排队
     * 4. 批量激活所有排队的技能
     * 5. 处理释放输入 —— 已激活技能转发释放事件
     * 6. 清空按下和释放缓存（按住缓存不清理，跨帧保持）
     *
     * @param DeltaTime   帧间隔时间
     * @param bGamePaused 游戏是否暂停
     */
    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

    /**
     * @brief 清空所有输入缓存
     *
     * 通常在输入被阻塞时调用（如过场动画、死亡、对话等状态）。
     * 同时清空按下、释放、按住三个缓存数组。
     */
    void ClearAbilityInput();

    /**
     * @brief 设置 Tag 关系映射表
     *
     * 用于定义技能 Tag 之间的阻挡/取消关系，控制技能间的互斥和打断行为。
     *
     * @param NewMapping 新的 Tag 关系映射配置
     */
    void SetTagRelationshipMapping(UNyotaAbilityTagRelationshipMapping *NewMapping);

protected:
    /**
     * @brief 将按下输入事件转发给已激活的技能实例
     *
     * 通过 PredictionKey 将 InputPressed 事件复制到技能实例，
     * 确保在蓄力、连招等需要持续接收输入的技能中，网络预测能够正确同步。
     *
     * @param Spec 接收到输入事件的技能规格
     */
    virtual void AbilitySpecInputPressed(FGameplayAbilitySpec &Spec) override;

    /**
     * @brief 将释放输入事件转发给已激活的技能实例
     *
     * 与 AbilitySpecInputPressed 镜像对称，转发释放事件。
     *
     * @param Spec 接收到输入事件的技能规格
     */
    virtual void AbilitySpecInputReleased(FGameplayAbilitySpec &Spec) override;

    /** Tag 关系映射表，定义技能 Tag 之间的阻挡/取消关系 */
    UPROPERTY()
    TObjectPtr<UNyotaAbilityTagRelationshipMapping> TagRelationshipMapping;

    // -- 输入缓存（每帧消费）--

    /** 本帧被按下的技能 Handle，ProcessAbilityInput 消费后清空 */
    TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

    /** 本帧被释放的技能 Handle，ProcessAbilityInput 消费后清空 */
    TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

    /** 当前处于按住状态的技能 Handle，跨帧持久，仅在 AbilityInputTagReleased 时移除 */
    TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

private:
    /**
     * @brief 检查并激活"授予即激活"的技能
     *
     * 检查技能的 AbilityTags 中是否包含 Ability_ActivateOnGive Tag，
     * 如果有则立即调用 TryActivateAbility。用于被动技能、光环等场景。
     *
     * @param AbilitySpec 要检查的技能规格
     */
    void HandleAutoActivateAbility(const FGameplayAbilitySpec &AbilitySpec);
};
