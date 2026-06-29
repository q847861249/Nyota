// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/NyotaExperienceManagerComponent.h"

#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"
#include "NyotaLogChannels.h"
#include "GameModes/NyotaExperienceActionSet.h"
#include "GameModes/NyotaExperienceDefinition.h"
#include "GameModes/NyotaExperienceManager.h"
#include "Net/UnrealNetwork.h"
#include "System/NyotaAssetManager.h"

UNyotaExperienceManagerComponent::UNyotaExperienceManagerComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
}

const UNyotaExperienceDefinition *UNyotaExperienceManagerComponent::GetCurrentExperienceChecked() const
{
    check(LoadState == ENyotaExperienceLoadState::Loaded);
    check(CurrentExperience != nullptr);
    return CurrentExperience;
}

bool UNyotaExperienceManagerComponent::IsExperienceLoaded() const
{
    return (LoadState == ENyotaExperienceLoadState::Loaded) && (CurrentExperience != nullptr);
}

void UNyotaExperienceManagerComponent::SetCurrentExperience(const UNyotaExperienceDefinition *InExperience)
{
    check(InExperience);
    check(LoadState == ENyotaExperienceLoadState::Unloaded);
    check(CurrentExperience == nullptr);

    CurrentExperience = InExperience;
    StartExperienceLoad();
}

void UNyotaExperienceManagerComponent::GetLifetimeReplicatedProps(
    TArray<class FLifetimeProperty> &OutLifetimeProps
) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, CurrentExperience);
}

void UNyotaExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnNyotaExperienceLoaded::FDelegate &&Delegate)
{
    if (IsExperienceLoaded())
    {
        Delegate.Execute(CurrentExperience);
    }
    else
    {
        OnExperienceLoaded.Add(MoveTemp(Delegate));
    }
}

void UNyotaExperienceManagerComponent::OnRep_CurrentExperience()
{
    StartExperienceLoad();
}

void UNyotaExperienceManagerComponent::StartExperienceLoad()
{
    check(CurrentExperience != nullptr);
    check(LoadState == ENyotaExperienceLoadState::Unloaded);

    UE_LOG(
        LogTemp,
        Log,
        TEXT("EXPERIENCE: StartExperienceLoad(CurrentExperience = %s, %s)"),
        *CurrentExperience->GetPrimaryAssetId().ToString(),
        *GetClientServerContextString(this)
    );

    LoadState = ENyotaExperienceLoadState::Loading;

    UNyotaAssetManager &AssetManager = UNyotaAssetManager::Get();

    TSet<FPrimaryAssetId> BundleAssetList;
    TSet<FSoftObjectPath> RawAssetList;

    BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
    for (const TObjectPtr<UNyotaExperienceActionSet> &ActionSet : CurrentExperience->ActionSets)
    {
        if (ActionSet != nullptr)
        {
            BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
        }
    }

    // Load assets associated with the experience

    TArray<FName> BundlesToLoad;
    BundlesToLoad.Add(FNyotaBundles::Equipped);

    //@TODO: Centralize this client/server stuff into the NyotaAssetManager
    const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
    const bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
    const bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
    if (bLoadClient)
    {
        BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
    }
    if (bLoadServer)
    {
        BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
    }

    TSharedPtr<FStreamableHandle> BundleLoadHandle = nullptr;
    if (BundleAssetList.Num() > 0)
    {
        BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(
            BundleAssetList.Array(),
            BundlesToLoad,
            {},
            false,
            FStreamableDelegate(),
            FStreamableManager::AsyncLoadHighPriority
        );
    }

    TSharedPtr<FStreamableHandle> RawLoadHandle = nullptr;
    if (RawAssetList.Num() > 0)
    {
        RawLoadHandle = AssetManager.LoadAssetList(
            RawAssetList.Array(),
            FStreamableDelegate(),
            FStreamableManager::AsyncLoadHighPriority,
            TEXT("StartExperienceLoad()")
        );
    }

    // If both async loads are running, combine them
    TSharedPtr<FStreamableHandle> Handle = nullptr;
    if (BundleLoadHandle.IsValid() && RawLoadHandle.IsValid())
    {
        Handle = AssetManager.GetStreamableManager().CreateCombinedHandle({BundleLoadHandle, RawLoadHandle});
    }
    else
    {
        Handle = BundleLoadHandle.IsValid() ? BundleLoadHandle : RawLoadHandle;
    }

    FStreamableDelegate OnAssetsLoadedDelegate =
        FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
    if (!Handle.IsValid() || Handle->HasLoadCompleted())
    {
        // Assets were already loaded, call the delegate now
        FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
    }
    else
    {
        Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);

        Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]() {
            OnAssetsLoadedDelegate.ExecuteIfBound();
        }));
    }

    // This set of assets gets preloaded, but we don't block the start of the experience based on it
    TSet<FPrimaryAssetId> PreloadAssetList;
    //@TODO: Determine assets to preload (but not blocking-ly)
    if (PreloadAssetList.Num() > 0)
    {
        AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
    }
}

void UNyotaExperienceManagerComponent::OnExperienceLoadComplete()
{
    check(LoadState == ENyotaExperienceLoadState::Loading);
    check(CurrentExperience != nullptr);

    UE_LOG(
        LogTemp,
        Log,
        TEXT("EXPERIENCE: OnExperienceLoadComplete(CurrentExperience = %s, %s)"),
        *CurrentExperience->GetPrimaryAssetId().ToString(),
        *GetClientServerContextString(this)
    );

    // find the URLs for our GameFeaturePlugins - filtering out dupes and ones that don't have a valid mapping
    GameFeaturePluginURLs.Reset();

    auto CollectGameFeaturePluginURLs =
        [This = this](const UPrimaryDataAsset *Context, const TArray<FString> &FeaturePluginList) {
            for (const FString &PluginName : FeaturePluginList)
            {
                FString PluginURL;
                if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, /*out*/ PluginURL))
                {
                    This->GameFeaturePluginURLs.AddUnique(PluginURL);
                }
                else
                {
                    ensureMsgf(
                        false,
                        TEXT(
                            "OnExperienceLoadComplete failed to find plugin URL from PluginName %s for experience %s - "
                            "fix data, ignoring for this run"
                        ),
                        *PluginName,
                        *Context->GetPrimaryAssetId().ToString()
                    );
                }
            }

            // 		// Add in our extra plugin
            // 		if (!CurrentPlaylistData->GameFeaturePluginToActivateUntilDownloadedContentIsPresent.IsEmpty())
            // 		{
            // 			FString PluginURL;
            // 			if
            // (UGameFeaturesSubsystem::Get().GetPluginURLByName(CurrentPlaylistData->GameFeaturePluginToActivateUntilDownloadedContentIsPresent,
            // PluginURL))
            // 			{
            // 				GameFeaturePluginURLs.AddUnique(PluginURL);
            // 			}
            // 		}
        };

    CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);
    for (const TObjectPtr<UNyotaExperienceActionSet> &ActionSet : CurrentExperience->ActionSets)
    {
        if (ActionSet != nullptr)
        {
            CollectGameFeaturePluginURLs(ActionSet, ActionSet->GameFeaturesToEnable);
        }
    }

    // Load and activate the features
    NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
    if (NumGameFeaturePluginsLoading > 0)
    {
        LoadState = ENyotaExperienceLoadState::LoadingGameFeatures;
        for (const FString &PluginURL : GameFeaturePluginURLs)
        {
            UNyotaExperienceManager::NotifyOfPluginActivation(PluginURL);
            UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(
                PluginURL,
                FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete)
            );
        }
    }
    else
    {
        OnExperienceFullLoadCompleted();
    }
}

void UNyotaExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult &Result)
{
    // decrement the number of plugins that are loading
    NumGameFeaturePluginsLoading--;

    if (NumGameFeaturePluginsLoading == 0)
    {
        OnExperienceFullLoadCompleted();
    }
}

void UNyotaExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
    check(LoadState != ENyotaExperienceLoadState::Loaded);

    LoadState = ENyotaExperienceLoadState::Loaded;

    // Broadcast to all listeners that the experience has finished loading
    OnExperienceLoaded.Broadcast(CurrentExperience);
}