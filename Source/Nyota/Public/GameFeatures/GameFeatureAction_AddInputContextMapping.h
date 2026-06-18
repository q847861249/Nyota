// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/GameFeatureAction_WorldActionBase.h"
#include "GameFeatureAction_AddInputContextMapping.generated.h"

struct FComponentRequestHandle;
class UInputMappingContext;
USTRUCT()
struct FInputMappingContextAndPriority
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AssetBundles = "Client,Server"))
    TSoftObjectPtr<UInputMappingContext> InputMapping;

    // 优先级较高的输入映射将优先于优先级较低的映射
    UPROPERTY(EditAnywhere, Category = "Input")
    int32 Priority = 0;

    // 如果为 True，那么这个 Mapping Context 将注册带有注册此 Game Feature 时的设置
    UPROPERTY(EditAnywhere, Category = "Input")
    bool bRegisterWithSetting = true;
};

/**
 * 添加 InputMappingContext 到本地玩家的增强输入系统中
 * 预计本地玩家将被设置为使用增强输入系统
 */
UCLASS()
class NYOTA_API UGameFeatureAction_AddInputContextMapping : public UGameFeatureAction_WorldActionBase
{
    GENERATED_BODY()

public:
    virtual void OnGameFeatureRegistering() override;

    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext &Context) override;

    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext &Context) override;

    virtual void OnGameFeatureUnregistering() override;

#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(class FDataValidationContext &Context) const override;
#endif

    UPROPERTY(EditAnywhere, Category = "Input")
    TArray<FInputMappingContextAndPriority> InputMappings;

private:
    struct FPerContextData
    {
        TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
        TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
    };

    TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

    // 委托句柄：用于监听 GameInstance 变化，以便注册 IMC
    FDelegateHandle RegisterInputContextMappingsForGameInstanceHandle;

    /**
     * @brief 将拥有的 Input Mapping Context 注册到输入注册表子系统。同时绑定 GameInstance
     * 启动事件以及本地玩家的添加/移除事件
     */
    void RegisterInputMappingContexts();

    /**
     * @brief 将拥有的 Input Mapping Context 注册到输入注册表子系统，针对指定的 GameInstance。该方法也会在 GameInstance
     * 启动时被调用。
     * @param GameInstance
     */
    void RegisterInputContextMappingsForGameInstance(UGameInstance *GameInstance);

    /**
     * @brief 将拥有的 Input Mapping Context
     * 注册到输入注册表子系统，针对指定的本地玩家。该方法也会在本地玩家被添加时调用。
     * @param LocalPlayer
     */
    void RegisterInputMappingContextForLocalPlayer(ULocalPlayer *LocalPlayer);

    /**
     * @brief 从输入注册表子系统中注销拥有的 Input Mapping Context。同时解绑 GameInstance
     * 启动事件以及本地玩家的添加/移除事件。
     */
    void UnregisterInputMappingContexts();

    /**
     * @brief 从输入注册表子系统中注销拥有的 Input Mapping Context，针对指定的 GameInstance。
     * @param GameInstance
     */
    void UnregisterInputContextMappingForGameInstance(UGameInstance *GameInstance);

    /**
     * @brief 从输入注册表子系统中注销拥有的 Input Mapping
     * Context，针对指定的本地玩家。该方法也会在本地玩家被移除时调用。
     * @param LocalPlayer
     */
    void UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer *LocalPlayer);

    virtual void AddToWorld(
        const FWorldContext &WorldContext, const FGameFeatureStateChangeContext &ChangeContext
    ) override;

    void Reset(FPerContextData &ActiveData);

    void HandleControllerExtension(AActor *Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

    void AddInputMappingForPlayer(UPlayer *Player, FPerContextData &ActiveData);

    void RemoveInputMapping(APlayerController *PlayerController, FPerContextData &ActiveData);
};
