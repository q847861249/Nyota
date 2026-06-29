// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "NyotaExperienceManagerComponent.generated.h"

namespace UE::GameFeatures
{
struct FResult;
}

class UNyotaExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNyotaExperienceLoaded, const UNyotaExperienceDefinition * /*Experience*/);

enum class ENyotaExperienceLoadState
{
    Unloaded,
    Loading,
    LoadingGameFeatures,
    LoadingChaosTestingDelay,
    ExecutingActions,
    Loaded,
    Deactivating
};

UCLASS()
class NYOTA_API UNyotaExperienceManagerComponent : public UGameStateComponent
{
    GENERATED_BODY()

public:
    UNyotaExperienceManagerComponent(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    // This returns the current experience if it is fully loaded, asserting otherwise
    // (i.e., if you called it too soon)
    const UNyotaExperienceDefinition *GetCurrentExperienceChecked() const;

    // Returns true if the experience is fully loaded
    bool IsExperienceLoaded() const;

    // Sets the current experience and starts loading it
    void SetCurrentExperience(const UNyotaExperienceDefinition *InExperience);

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const override;

    // Ensures the delegate is called once the experience has been loaded
    // If the experience has already loaded, calls the delegate immediately
    void CallOrRegister_OnExperienceLoaded(FOnNyotaExperienceLoaded::FDelegate &&Delegate);

private:
    UFUNCTION()
    void OnRep_CurrentExperience();

    void StartExperienceLoad();

    void OnExperienceLoadComplete();

    void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult &Result);

    void OnExperienceFullLoadCompleted();

    UPROPERTY(ReplicatedUsing = OnRep_CurrentExperience)
    TObjectPtr<const UNyotaExperienceDefinition> CurrentExperience;

    ENyotaExperienceLoadState LoadState = ENyotaExperienceLoadState::Unloaded;

    int32 NumGameFeaturePluginsLoading = 0;

    TArray<FString> GameFeaturePluginURLs;

    /** Delegate called when the experience has finished loading */
    FOnNyotaExperienceLoaded OnExperienceLoaded;
};
