// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/NyotaHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "DataInterface/NiagaraDataInterfaceActorComponent.h"
#include "Slate/SGameLayerManager.h"
#include "System/NyotaAssetManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputContextMapping)

#define LOCTEXT_NAMESPACE "GameFeatures"

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureRegistering()
{
    Super::OnGameFeatureRegistering();

    // Game Feature 首次注册时，绑定全局 GameInstance 启动委托，
    // 确保无论当前是否已有 GameInstance，后续新启动的 GameInstance 都能自动注册输入映射
    RegisterInputMappingContexts();
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureActivating(FGameFeatureActivatingContext &Context)
{
    // 获取或创建当前上下文的运行时数据
    FPerContextData &ActiveData = ContextData.FindOrAdd(Context);

    // 如果数据不为空（理论上此时不应有残留数据），先做一次重置以保证状态干净
    if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty()) || !ensure(ActiveData.ControllersAddedTo.IsEmpty()))
    {
        Reset(ActiveData);
    }

    Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext &Context)
{
    Super::OnGameFeatureDeactivating(Context);

    // 停用时清理该上下文的所有运行时数据（移除扩展句柄、清理已添加的控制器映射）
    FPerContextData *ActiveData = ContextData.Find(Context);
    if (ensure(ActiveData))
    {
        Reset(*ActiveData);
    }
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureUnregistering()
{
    Super::OnGameFeatureUnregistering();

    // Game Feature 被卸载时，解绑全局委托并清理所有 GameInstance 的注册
    UnregisterInputMappingContexts();
}

EDataValidationResult UGameFeatureAction_AddInputContextMapping::IsDataValid(
    class FDataValidationContext &Context
) const
{
    EDataValidationResult Result =
        CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

    int32 Index = 0;

    // 遍历所有配置的输入映射条目，检查是否有空引用
    for (const FInputMappingContextAndPriority &Entry : InputMappings)
    {
        if (Entry.InputMapping.IsNull())
        {
            Result = EDataValidationResult::Invalid;
            Context.AddError(FText::Format(LOCTEXT("NullInputMapping", "Null InputMapping at index {0}."), Index));
        }

        ++Index;
    }

    return Result;
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts()
{
    // 绑定全局委托：当任意 GameInstance 启动时，自动为其注册输入映射
    RegisterInputContextMappingsForGameInstanceHandle =
        FWorldDelegates::OnStartGameInstance.AddUObject(this, &ThisClass::RegisterInputContextMappingsForGameInstance);

    // 遍历当前已存在的所有 World Context，为已运行的 GameInstance 补注册
    const TIndirectArray<FWorldContext> &WorldContexts = GEngine->GetWorldContexts();
    for (TIndirectArray<FWorldContext>::TConstIterator WorldContextIterator = WorldContexts.CreateConstIterator();
         WorldContextIterator;
         ++WorldContextIterator)
    {
        RegisterInputContextMappingsForGameInstance(WorldContextIterator->OwningGameInstance);
    }
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputContextMappingsForGameInstance(UGameInstance *GameInstance)
{
    // 仅当 GameInstance 有效且尚未绑定过事件时才注册（防止重复绑定）
    if (GameInstance != nullptr && !GameInstance->OnLocalPlayerAddedEvent.IsBoundToObject(this))
    {
        // 绑定本地玩家添加/移除事件，实现动态注册/注销输入映射
        GameInstance->OnLocalPlayerAddedEvent.AddUObject(this, &ThisClass::RegisterInputMappingContextsForLocalPlayer);
        GameInstance->OnLocalPlayerRemovedEvent.AddUObject(
            this, &ThisClass::UnregisterInputMappingContextsForLocalPlayer
        );

        // 为该 GameInstance 中已存在的所有本地玩家补注册输入映射
        for (TArray<ULocalPlayer *>::TConstIterator LocalPlayerIterator = GameInstance->GetLocalPlayerIterator();
             LocalPlayerIterator;
             ++LocalPlayerIterator)
        {
            RegisterInputMappingContextsForLocalPlayer(*LocalPlayerIterator);
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContextsForLocalPlayer(ULocalPlayer *LocalPlayer)
{
    if (ensure(LocalPlayer))
    {
        UNyotaAssetManager &AssetManager = UNyotaAssetManager::Get();

        // 获取增强输入子系统
        if (UEnhancedInputLocalPlayerSubsystem *EISubsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            // 通过 UserSettings 注册 IMC，这样用户可以后续通过设置菜单自定义按键绑定
            if (UEnhancedInputUserSettings *Settings = EISubsystem->GetUserSettings())
            {
                for (const FInputMappingContextAndPriority &Entry : InputMappings)
                {
                    // 跳过 bRegisterWithSetting == false 的条目（它们由其他逻辑单独处理）
                    if (!Entry.bRegisterWithSetting)
                    {
                        continue;
                    }

                    // 通过资产管理器同步加载 IMC 资产并注册到用户设置中
                    if (UInputMappingContext *IMC = AssetManager.GetAsset(Entry.InputMapping))
                    {
                        Settings->RegisterInputMappingContext(IMC);
                    }
                }
            }
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContexts()
{
    // 解绑全局 GameInstance 启动委托，不再监听新 GameInstance 的创建
    FWorldDelegates::OnStartGameInstance.Remove(RegisterInputContextMappingsForGameInstanceHandle);
    RegisterInputContextMappingsForGameInstanceHandle.Reset();

    // 遍历当前所有 GameInstance，逐一清理注册的输入映射和事件绑定
    const TIndirectArray<FWorldContext> &WorldContexts = GEngine->GetWorldContexts();
    for (TIndirectArray<FWorldContext>::TConstIterator WorldContextIterator = WorldContexts.CreateConstIterator();
         WorldContextIterator;
         ++WorldContextIterator)
    {
        UnregisterInputContextMappingForGameInstance(WorldContextIterator->OwningGameInstance);
    }
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputContextMappingForGameInstance(
    UGameInstance *GameInstance
)
{
    if (GameInstance != nullptr)
    {
        // 解绑该 GameInstance 上的本地玩家添加/移除事件
        GameInstance->OnLocalPlayerAddedEvent.RemoveAll(this);
        GameInstance->OnLocalPlayerRemovedEvent.RemoveAll(this);

        // 为该 GameInstance 中所有已存在的本地玩家注销输入映射
        for (TArray<ULocalPlayer *>::TConstIterator LocalPlayerIterator = GameInstance->GetLocalPlayerIterator();
             LocalPlayerIterator;
             ++LocalPlayerIterator)
        {
            UnregisterInputMappingContextsForLocalPlayer(*LocalPlayerIterator);
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer *LocalPlayer)
{
    if (ensure(LocalPlayer))
    {
        if (UEnhancedInputLocalPlayerSubsystem *EISubsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            if (UEnhancedInputUserSettings *Settings = EISubsystem->GetUserSettings())
            {
                for (const FInputMappingContextAndPriority &Entry : InputMappings)
                {
                    // 跳过未通过 UserSettings 注册的条目
                    if (!Entry.bRegisterWithSetting)
                    {
                        continue;
                    }

                    // 注意：此处使用 Get() 直接获取已加载的资产（而非通过 AssetManager 加载），
                    // 因为注销时资产应当已被加载
                    if (UInputMappingContext *IMC = Entry.InputMapping.Get())
                    {
                        Settings->UnregisterInputMappingContext(IMC);
                    }
                }
            }
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::AddToWorld(
    const FWorldContext &WorldContext, const FGameFeatureStateChangeContext &ChangeContext
)
{
    UWorld *World = WorldContext.World();
    UGameInstance *GameInstance = WorldContext.OwningGameInstance;
    FPerContextData &ActiveData = ContextData.FindOrAdd(ChangeContext);

    // 仅在游戏世界中注册控制器扩展处理器（跳过编辑器预览世界等非游戏世界）
    if (GameInstance != nullptr && World != nullptr && World->IsGameWorld())
    {
        if (UGameFrameworkComponentManager *ComponentManager =
                UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
        {
            // 创建扩展处理器委托：当 APlayerController 被添加/移除时回调 HandleControllerExtension
            UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate =
                UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
                    this, &ThisClass::HandleControllerExtension, ChangeContext
                );
            TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
                ComponentManager->AddExtensionHandler(APlayerController::StaticClass(), AddAbilitiesDelegate);

            // 保存扩展句柄，以便在停用/重置时移除
            ActiveData.ExtensionRequestHandles.Add(ExtensionRequestHandle);
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::Reset(FPerContextData &ActiveData)
{
    // 清空扩展请求句柄，释放对 GameFrameworkComponentManager 的回调注册
    ActiveData.ExtensionRequestHandles.Empty();

    // 遍历所有已添加输入映射的控制器，逐一移除
    while (!ActiveData.ControllersAddedTo.IsEmpty())
    {
        TWeakObjectPtr<APlayerController> ControllerPtr = ActiveData.ControllersAddedTo.Top();
        if (ControllerPtr.IsValid())
        {
            // 控制器仍然有效：移除该控制器的输入映射
            RemoveInputMapping(ControllerPtr.Get(), ActiveData);
        }
        else
        {
            // 控制器已被销毁（弱引用失效）：直接从列表中弹出
            ActiveData.ControllersAddedTo.Pop();
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::HandleControllerExtension(
    AActor *Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext
)
{
    APlayerController *AsController = CastChecked<APlayerController>(Actor);
    FPerContextData &ActiveData = ContextData.FindOrAdd(ChangeContext);

    // 根据事件类型决定是添加还是移除输入映射
    if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved ||
        EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
    {
        // 控制器扩展被移除或接收器被移除：移除输入映射
        RemoveInputMapping(AsController, ActiveData);
    }
    else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded ||
             EventName == UNyotaHeroComponent::NAME_BindInputsNow)
    {
        // 控制器扩展被添加或英雄组件请求绑定输入：添加输入映射
        AddInputMappingForPlayer(AsController->GetLocalPlayer(), ActiveData);
    }
}

void UGameFeatureAction_AddInputContextMapping::AddInputMappingForPlayer(UPlayer *Player, FPerContextData &ActiveData)
{
    if (ULocalPlayer *LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        // 获取增强输入子系统，将配置的所有 IMC 按优先级添加
        if (UEnhancedInputLocalPlayerSubsystem *InputSystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            for (const FInputMappingContextAndPriority &Entry : InputMappings)
            {
                // 注意：此处使用 Get() 直接获取已加载的资产，
                // 因为 IMC 资产应在 Game Feature 激活时已被预加载
                if (const UInputMappingContext *IMC = Entry.InputMapping.Get())
                {
                    InputSystem->AddMappingContext(IMC, Entry.Priority);
                }
            }
        }
        else
        {
            // 增强输入子系统不可用通常是项目配置问题 —— 记录错误以便排查
            UE_LOG(
                LogGameFeatures,
                Error,
                TEXT(
                    "Failed to find `UEnhancedInputLocalPlayerSubsystem` for local player. Input mappings will not be "
                    "added. Make sure you're set to use the EnhancedInput system via config file."
                )
            );
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::RemoveInputMapping(
    APlayerController *PlayerController, FPerContextData &ActiveData
)
{
    if (ULocalPlayer *LocalPlayer = PlayerController->GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem *InputSystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            // 遍历所有配置的 IMC，从子系统中移除
            for (const FInputMappingContextAndPriority &Entry : InputMappings)
            {
                if (const UInputMappingContext *IMC = Entry.InputMapping.Get())
                {
                    InputSystem->RemoveMappingContext(IMC);
                }
            }
        }
    }

    // 从跟踪列表中移除该控制器
    ActiveData.ControllersAddedTo.Remove(PlayerController);
}

#undef LOCTEXT_NAMESPACE