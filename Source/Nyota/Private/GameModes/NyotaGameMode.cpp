// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/NyotaGameMode.h"

#include "Character/BasePlayerState.h"
#include "Character/NyotaPawnData.h"
#include "Character/NyotaPawnExtensionComponent.h"
#include "GameModes/NyotaExperienceDefinition.h"
#include "GameModes/NyotaExperienceManagerComponent.h"
#include "System/NyotaAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NyotaGameMode)

ANyotaGameMode::ANyotaGameMode(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

const UNyotaPawnData *ANyotaGameMode::GetPawnDataForController(const AController *InController) const
{
    // ① 优先：PlayerState 上可能已由 Experience 回调设置了 PawnData
    if (InController != nullptr)
    {
        if (const ABasePlayerState *NyotaPS = InController->GetPlayerState<ABasePlayerState>())
        {
            if (const UNyotaPawnData *PawnData = NyotaPS->GetPawnData<UNyotaPawnData>())
            {
                return PawnData;
            }
        }
    }

    // ② 次优：从当前 Experience 的 DefaultPawnData 获取
    if (const UNyotaExperienceManagerComponent *ExperienceComponent = GetExperienceManagerComponent())
    {
        if (ExperienceComponent->IsExperienceLoaded())
        {
            if (const UNyotaExperienceDefinition *Experience = ExperienceComponent->GetCurrentExperienceChecked())
            {
                if (Experience->DefaultPawnData)
                {
                    return Experience->DefaultPawnData;
                }
            }
        }
    }

    // ③ 兜底：AssetManager 全局默认
    return UNyotaAssetManager::Get().GetDefaultPawnData();
}

APawn *ANyotaGameMode::SpawnDefaultPawnAtTransform_Implementation(
    AController *NewPlayer, const FTransform &SpawnTransform
)
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Instigator = GetInstigator();
    SpawnInfo.ObjectFlags |= RF_Transient; // Never save the default player pawns into a map.
    SpawnInfo.bDeferConstruction = true;

    if (UClass *PawnClass = GetDefaultPawnClassForController(NewPlayer))
    {
        if (APawn *SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
        {
            if (UNyotaPawnExtensionComponent *PawnExtComp =
                    UNyotaPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
            {
                if (const UNyotaPawnData *PawnData = GetPawnDataForController(NewPlayer))
                {
                    PawnExtComp->SetPawnData(PawnData);
                }
                else
                {
                    UE_LOG(
                        LogTemp,
                        Error,
                        TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."),
                        *GetNameSafe(SpawnedPawn)
                    );
                }
            }

            SpawnedPawn->FinishSpawning(SpawnTransform);

            return SpawnedPawn;
        }
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."),
            *GetNameSafe(PawnClass),
            *SpawnTransform.ToHumanReadableString()
        );
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
    }

    return nullptr;
}

void ANyotaGameMode::InitGameState()
{
    Super::InitGameState();

    UNyotaExperienceManagerComponent *ExperienceComponent =
        GameState->FindComponentByClass<UNyotaExperienceManagerComponent>();
    check(ExperienceComponent);

    // Listen for the experience load to complete
    ExperienceComponent->CallOrRegister_OnExperienceLoaded(
        FOnNyotaExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded)
    );

    // Dispatch the default experience
    if (const UNyotaExperienceDefinition *Experience = DefaultExperience.LoadSynchronous())
    {
        ExperienceComponent->SetCurrentExperience(Experience);
    }
}

const UNyotaExperienceManagerComponent *ANyotaGameMode::GetExperienceManagerComponent() const
{
    if (const AGameStateBase *GS = GameState)
    {
        return GS->FindComponentByClass<UNyotaExperienceManagerComponent>();
    }
    return nullptr;
}

void ANyotaGameMode::OnExperienceLoaded(const UNyotaExperienceDefinition *CurrentExperience)
{
    // Experience 加载完成后，如果当前已有玩家在等待 PawnData，可以在这里统一处理
    // 目前 PawnData 的初始化由 PlayerState::OnExperienceLoaded 回调自行处理
}
