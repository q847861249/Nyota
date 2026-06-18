// Fill out your copyright notice in the Description page of Project Settings.

#include "System/NyotaAssetManager.h"

#include "Character/NyotaPawnData.h"
#include "System/NyotaAssetManagerStartupJob.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NyotaAssetManager)

//////////////////////////////////////////////////////////////////////

#define STARTUP_JOB_WEIGHTED(JobFunc, JobWeight)                                                                       \
    StartupJobs.Add(FNyotaAssetManagerStartupJob(                                                                      \
        #JobFunc,                                                                                                      \
        [this](const FNyotaAssetManagerStartupJob &StartupJob, TSharedPtr<FStreamableHandle> &LoadHandle) {            \
            JobFunc;                                                                                                   \
        },                                                                                                             \
        JobWeight                                                                                                      \
    ))
#define STARTUP_JOB(JobFunc) STARTUP_JOB_WEIGHTED(JobFunc, 1.f)

UNyotaAssetManager::UNyotaAssetManager()
{
    DefaultPawnData = nullptr;
}

UNyotaAssetManager &UNyotaAssetManager::Get()
{
    check(GEngine);

    if (UNyotaAssetManager *Singleton = Cast<UNyotaAssetManager>(GEngine->AssetManager))
    {
        return *Singleton;
    }

    UE_LOG(
        LogTemp,
        Fatal,
        TEXT("Invalid AssetManagerClassName in DefaultEngine.ini, It must be set to NyotaAssetManager !")
    );

    return *NewObject<UNyotaAssetManager>();
}

void UNyotaAssetManager::DumpLoadedAssets()
{
    UE_LOG(LogTemp, Log, TEXT("========== Start Dumping Loaded Assets =========="));

    for (const UObject *LoadedAsset : Get().LoadedAssets)
    {
        UE_LOG(LogTemp, Log, TEXT("  %s"), *GetNameSafe(LoadedAsset));
    }

    UE_LOG(LogTemp, Log, TEXT("... %d assets in loaded pool"), Get().LoadedAssets.Num());
    UE_LOG(LogTemp, Log, TEXT("========== Finish Dumping Loaded Assets =========="));
}

const UNyotaGameData &UNyotaAssetManager::GetGameData()
{
    return GetOrLoadTypedGameData<UNyotaGameData>(NyotaGameDataPath);
}

const UNyotaPawnData *UNyotaAssetManager::GetDefaultPawnData() const
{
    return GetAsset(DefaultPawnData);
}

UObject *UNyotaAssetManager::SynchronousLoadAsset(const FSoftObjectPath &AssetPath)
{
    if (AssetPath.IsValid())
    {
        TUniquePtr<FScopeLogTime> LogTimePtr;

        if (ShouldLogAssetLoads())
        {
            LogTimePtr = MakeUnique<FScopeLogTime>(
                *FString::Printf(TEXT("Synchronously loaded asset: %s"), *AssetPath.ToString())
            );
        }

        if (IsInitialized())
        {
            return GetStreamableManager().LoadSynchronous(AssetPath, false);
        }

        // 如果 AssetManager 还未准备就绪，使用 LoadObject。
        return AssetPath.TryLoad();
    }

    return nullptr;
}

bool UNyotaAssetManager::ShouldLogAssetLoads()
{
    // 检查命令行是否带了 -LogAssetLoads 参数
    static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));

    return bLogAssetLoads;
}

void UNyotaAssetManager::AddLoadedAsset(const UObject *Asset)
{
    if (ensureAlways(Asset))
    {
        FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);

        LoadedAssets.Add(Asset);
    }
}

void UNyotaAssetManager::StartInitialLoading()
{
    SCOPED_BOOT_TIMING("UNyotaAssetManager::StartInitialLoading");

    // 这会完成所有扫描，即使负载延迟，现在也需要这样做
    Super::StartInitialLoading();

    STARTUP_JOB(InitializeGameplayCueManager());
    
    {
        // Load base game data asset
        STARTUP_JOB_WEIGHTED(GetGameData(), 25.f);
    }

    // Run all the queued up startup jobs
    DoAllStartupJobs();
}

void UNyotaAssetManager::PreBeginPIE(bool bStartSimulate)
{
    Super::PreBeginPIE(bStartSimulate);
}

UPrimaryDataAsset *UNyotaAssetManager::LoadGameDataOfClass(
    TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset> &DataClassPath,
    FPrimaryAssetType PrimaryAssetType
)
{
    return nullptr;
}

void UNyotaAssetManager::DoAllStartupJobs()
{
}

void UNyotaAssetManager::InitializeGameplayCueManager()
{
}

void UNyotaAssetManager::UpdateInitialGameContentLoadPercent(float GameContentPercent)
{
}
