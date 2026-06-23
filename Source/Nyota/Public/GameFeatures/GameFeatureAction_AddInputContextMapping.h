// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/GameFeatureAction_WorldActionBase.h"
#include "GameFeatureAction_AddInputContextMapping.generated.h"

struct FComponentRequestHandle;
class UInputMappingContext;

/**
 * @brief 输入映射上下文及其优先级配置
 *
 * 将 UInputMappingContext 资产与优先级和注册设置绑定在一起。
 * 在 Game Feature 激活时，这些映射会被添加到本地玩家的增强输入子系统中。
 */
USTRUCT()
struct FInputMappingContextAndPriority
{
    GENERATED_BODY()

    /** 指向 UInputMappingContext 资产的软引用，支持异步加载 */
    UPROPERTY(EditAnywhere, Category = "Input", meta = (AssetBundles = "Client,Server"))
    TSoftObjectPtr<UInputMappingContext> InputMapping;

    /** 优先级较高的输入映射将优先于优先级较低的映射进行输入处理 */
    UPROPERTY(EditAnywhere, Category = "Input")
    int32 Priority = 0;

    /** 如果为 True，那么这个 Mapping Context 将在注册时携带增强输入用户设置 */
    UPROPERTY(EditAnywhere, Category = "Input")
    bool bRegisterWithSetting = true;
};

/**
 * @brief 添加 InputMappingContext 到本地玩家的增强输入系统中
 *
 * 作为 Game Feature Action，负责在 Game Feature 生命周期的不同阶段
 * 注册/注销增强输入映射上下文。支持：
 * - 监听 GameInstance 启动事件，自动为新 GameInstance 注册映射
 * - 监听本地玩家添加/移除事件，动态管理每个玩家的输入映射
 * - 通过 GameFrameworkComponentManager 扩展控制器，在控制器添加/移除时处理映射
 *
 * 预计本地玩家将被设置为使用增强输入系统（需在项目配置中启用 EnhancedInput）。
 */
UCLASS()
class NYOTA_API UGameFeatureAction_AddInputContextMapping : public UGameFeatureAction_WorldActionBase
{
    GENERATED_BODY()

public:
    //~ Begin UGameFeatureAction Interface
    /** Game Feature 注册时调用：绑定全局 GameInstance 启动事件，开始监听输入映射注册时机 */
    virtual void OnGameFeatureRegistering() override;

    /** Game Feature 激活时调用：为当前上下文初始化 FPerContextData */
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext &Context) override;

    /** Game Feature 停用时调用：重置当前上下文的活跃数据，移除已添加的输入映射 */
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext &Context) override;

    /** Game Feature 注销时调用：解绑全局 GameInstance 启动事件，清理所有注册 */
    virtual void OnGameFeatureUnregistering() override;
    //~ End UGameFeatureAction Interface

#if WITH_EDITOR
    /** 编辑器数据校验：检查所有 InputMapping 条目是否配置了有效的资产引用 */
    virtual EDataValidationResult IsDataValid(class FDataValidationContext &Context) const override;
#endif

    /** 需要在 Game Feature 激活时添加的输入映射上下文列表 */
    UPROPERTY(EditAnywhere, Category = "Input")
    TArray<FInputMappingContextAndPriority> InputMappings;

private:
    /** 每个 Game Feature 状态上下文的运行时数据 */
    struct FPerContextData
    {
        /** 组件扩展请求句柄，用于向 GameFrameworkComponentManager 注册控制器扩展监听 */
        TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;

        /** 已添加过输入映射的玩家控制器列表（弱引用），用于在停用时清理 */
        TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
    };

    /** 以 GameFeatureStateChangeContext 为键的上下文数据映射表 */
    TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

    /** 委托句柄：用于监听 FWorldDelegates::OnStartGameInstance，以便在新 GameInstance 启动时自动注册 IMC */
    FDelegateHandle RegisterInputContextMappingsForGameInstanceHandle;

    // ──────────────────────────────────────────────
    // 注册 (Register) 方法
    // ──────────────────────────────────────────────

    /**
     * @brief 全局注册入口。绑定 FWorldDelegates::OnStartGameInstance 委托，
     * 并为当前所有已存在的 GameInstance 注册输入映射。
     */
    void RegisterInputMappingContexts();

    /**
     * @brief 为指定 GameInstance 绑定本地玩家的添加/移除事件，
     * 并为该 GameInstance 中已存在的全部本地玩家注册输入映射。
     * @param GameInstance 目标 GameInstance，若为 nullptr 则直接返回
     */
    void RegisterInputContextMappingsForGameInstance(UGameInstance *GameInstance);

    /**
     * @brief 为指定本地玩家注册输入映射上下文。
     * 将配置的 InputMapping 资产通过 UEnhancedInputUserSettings 注册到增强输入子系统中。
     * 仅处理 bRegisterWithSetting == true 的条目。
     * @param LocalPlayer 目标本地玩家
     */
    void RegisterInputMappingContextsForLocalPlayer(ULocalPlayer *LocalPlayer);

    // ──────────────────────────────────────────────
    // 注销 (Unregister) 方法
    // ──────────────────────────────────────────────

    /**
     * @brief 全局注销入口。从 FWorldDelegates::OnStartGameInstance 解绑委托句柄，
     * 并为当前所有已存在的 GameInstance 注销输入映射。
     */
    void UnregisterInputMappingContexts();

    /**
     * @brief 解绑指定 GameInstance 上的本地玩家添加/移除事件，
     * 并为该 GameInstance 中已存在的全部本地玩家注销输入映射。
     * @param GameInstance 目标 GameInstance，若为 nullptr 则直接返回
     */
    void UnregisterInputContextMappingForGameInstance(UGameInstance *GameInstance);

    /**
     * @brief 为指定本地玩家注销输入映射上下文。
     * 将配置的 InputMapping 资产通过 UEnhancedInputUserSettings 从增强输入子系统中移除。
     * 仅处理 bRegisterWithSetting == true 的条目。
     * @param LocalPlayer 目标本地玩家
     */
    void UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer *LocalPlayer);

    // ──────────────────────────────────────────────
    // 运行时管理
    // ──────────────────────────────────────────────

    /**
     * @brief 当 Game Feature 被添加到世界时调用。
     * 通过 GameFrameworkComponentManager 注册 APlayerController 的扩展处理器，
     * 以监听控制器的添加/移除事件。
     */
    virtual void AddToWorld(
        const FWorldContext &WorldContext, const FGameFeatureStateChangeContext &ChangeContext
    ) override;

    /** 重置活跃数据：清空扩展请求句柄并移除所有已添加的控制器映射 */
    void Reset(FPerContextData &ActiveData);

    /**
     * @brief 控制器扩展事件处理器。
     * 根据事件名决定是添加还是移除输入映射：
     * - ExtensionAdded / BindInputsNow → 添加映射
     * - ExtensionRemoved / ReceiverRemoved → 移除映射
     */
    void HandleControllerExtension(AActor *Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

    /**
     * @brief 为指定玩家添加所有配置的输入映射上下文到增强输入子系统。
     * @param Player 目标玩家（将尝试 Cast 到 ULocalPlayer）
     * @param ActiveData 当前上下文的活跃数据（用于跟踪已添加的控制器）
     */
    void AddInputMappingForPlayer(UPlayer *Player, FPerContextData &ActiveData);

    /**
     * @brief 从增强输入子系统中移除指定玩家控制器的所有输入映射上下文。
     * @param PlayerController 目标玩家控制器
     * @param ActiveData 当前上下文的活跃数据（用于清理 ControllerAddedTo 列表）
     */
    void RemoveInputMapping(APlayerController *PlayerController, FPerContextData &ActiveData);
};
