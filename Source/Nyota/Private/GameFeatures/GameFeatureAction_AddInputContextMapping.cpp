// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "System/NyotaAssetManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputContextMapping)

#define LOCTEXT_NAMESPACE "GameFeatures"

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureRegistering()
{
    Super::OnGameFeatureRegistering();

    RegisterInputMappingContexts();
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureActivating(FGameFeatureActivatingContext &Context)
{
    FPerContextData &ActiveData = ContextData.FindOrAdd(Context);
    if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty()) || !ensure(ActiveData.ControllersAddedTo.IsEmpty()))
    {
        Reset(ActiveData);
    }

    Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext &Context)
{
    Super::OnGameFeatureDeactivating(Context);

    FPerContextData *ActiveData = ContextData.Find(Context);
    if (ensure(ActiveData))
    {
        Reset(*ActiveData);
    }
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureUnregistering()
{
    Super::OnGameFeatureUnregistering();

    UnregisterInputMappingContexts();
}

EDataValidationResult UGameFeatureAction_AddInputContextMapping::IsDataValid(
    class FDataValidationContext &Context
) const
{
    EDataValidationResult Result =
        CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

    int32 Index = 0;

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
    RegisterInputContextMappingsForGameInstanceHandle =
        FWorldDelegates::OnStartGameInstance.AddUObject(this, &ThisClass::RegisterInputContextMappingsForGameInstance);

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
    if (GameInstance != nullptr && !GameInstance->OnLocalPlayerAddedEvent.IsBoundToObject(this))
    {
        GameInstance->OnLocalPlayerAddedEvent.AddUObject(this, &ThisClass::RegisterInputMappingContextForLocalPlayer);
        GameInstance->OnLocalPlayerRemovedEvent.AddUObject(
            this, &ThisClass::UnregisterInputMappingContextsForLocalPlayer
        );

        for (TArray<ULocalPlayer *>::TConstIterator LocalPlayerIterator = GameInstance->GetLocalPlayerIterator();
             LocalPlayerIterator;
             ++LocalPlayerIterator)
        {
            RegisterInputMappingContextForLocalPlayer(*LocalPlayerIterator);
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContextForLocalPlayer(ULocalPlayer *LocalPlayer)
{
    if (ensure(LocalPlayer))
    {
        UNyotaAssetManager &AssetManager = UNyotaAssetManager::Get();

        if (UEnhancedInputLocalPlayerSubsystem *EISubsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            if (UEnhancedInputUserSettings *Settings = EISubsystem->GetUserSettings())
            {
                for (const FInputMappingContextAndPriority &Entry : InputMappings)
                {
                    // 跳过不想注册的条目
                    if (!Entry.bRegisterWithSetting)
                    {
                        continue;
                    }

                    // 注册这个带设置的 IMC
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
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputContextMappingForGameInstance(
    UGameInstance *GameInstance
)
{
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer *LocalPlayer)
{
}

void UGameFeatureAction_AddInputContextMapping::AddToWorld(
    const FWorldContext &WorldContext, const FGameFeatureStateChangeContext &ChangeContext
)
{
}

void UGameFeatureAction_AddInputContextMapping::Reset(FPerContextData &ActiveData)
{
}

void UGameFeatureAction_AddInputContextMapping::HandleControllerExtension(
    AActor *Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext
)
{
}

void UGameFeatureAction_AddInputContextMapping::AddInputMappingForPlayer(UPlayer *Player, FPerContextData &ActiveData)
{
}

void UGameFeatureAction_AddInputContextMapping::RemoveInputMapping(
    APlayerController *PlayerController, FPerContextData &ActiveData
)
{
}

#undef LOCTEXT_NAMESPACE