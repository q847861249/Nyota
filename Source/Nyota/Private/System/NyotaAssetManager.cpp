// Fill out your copyright notice in the Description page of Project Settings.

#include "System/NyotaAssetManager.h"

#include "AbilitySystem/NyotaGameplayCueManager.h"
#include "Character/NyotaPawnData.h"
#include "System/NyotaAssetManagerStartupJob.h"
#include "System/NyotaGameData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NyotaAssetManager)

const FName FNyotaBundles::Equipped(TEXT("Equipped"));

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

    if (NyotaGameDataPath.IsValid())
    {
        // Load base game data asset
        STARTUP_JOB_WEIGHTED(GetGameData(), 25.f);
    }

    // Run all the queued up startup jobs
    DoAllStartupJobs();
}

#if WITH_EDITOR
void UNyotaAssetManager::PreBeginPIE(bool bStartSimulate)
{
    Super::PreBeginPIE(bStartSimulate);

    if (NyotaGameDataPath.IsValid())
    {
        FScopedSlowTask SlowTask(0, NSLOCTEXT("NyotaEditor", "BeginLoadingPIEData", "Loading PIE Data"));
        constexpr bool bShowCancelButton = false;
        constexpr bool bAllowInPIE = true;
        SlowTask.MakeDialog(bShowCancelButton, bAllowInPIE);

        GetGameData();

        // 刻意放在 GetGameData() 之后，避免 GameData 加载耗时被计入此计时器
        SCOPE_LOG_TIME_IN_SECONDS(TEXT("PreBeginPIE asset preloading complete"), nullptr)
    }
}
#endif

UPrimaryDataAsset *UNyotaAssetManager::LoadGameDataOfClass(
    TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset> &DataClassPath,
    FPrimaryAssetType PrimaryAssetType
)
{
    UPrimaryDataAsset *Asset = nullptr;

    DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Loading GameData Object"), STAT_GameData, STATGROUP_LoadTime);

    if (!DataClassPath.IsNull())
    {
#if WITH_EDITOR
        FScopedSlowTask SlowTask(
            0,
            FText::Format(
                NSLOCTEXT("NyotaEditor", "BeginLoadingGameDataTask", "Loading GameData {0}"),
                FText::FromName(DataClass->GetFName())
            )
        );
        constexpr bool bShowCancelButton = false;
        constexpr bool bAllowInPIE = true;
        SlowTask.MakeDialog(bShowCancelButton, bAllowInPIE);
#endif

        UE_LOG(LogTemp, Log, TEXT("Loading GameData: %s ..."), *DataClassPath.ToString());

        SCOPE_LOG_TIME_IN_SECONDS(TEXT("    ... GameData loaded!"), nullptr)

        if (GIsEditor)
        {
            // ──── 编辑器路径 ────
            Asset = DataClassPath.LoadSynchronous();     // 1: 同步加载主资产
            LoadPrimaryAssetsWithType(PrimaryAssetType); // 2: 异步加载关联资产组
        }
        else
        {
            // ──── 运行时路径 ────
            TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType); // 1: 异步加载全部

            if (Handle.IsValid())
            {
                Handle->WaitUntilComplete(0.0f, false); // 2: 等待完成

                Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset()); // 3: 取出主资产
            }
        }
    }

    if (Asset)
    {
        GameDataMap.Add(DataClass, Asset);
    }
    else
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "Failed to load GameData asset at %s. Type %s. This is not recoverable and likely means you do not "
                "have the correct data to run %s."
            ),
            *DataClassPath.ToString(),
            *PrimaryAssetType.ToString(),
            FApp::GetProjectName()
        );
    }

    return Asset;
}

void UNyotaAssetManager::DoAllStartupJobs()
{
    SCOPED_BOOT_TIMING("UNyotaAssetManager::DoAllStartupJobs");

    const double AllStartupJobsStartTime = FPlatformTime::Seconds();

    if (IsRunningDedicatedServer())
    {
        // 不需要周期性进度更新，直接执行任务
        for (const FNyotaAssetManagerStartupJob &StartupJob : StartupJobs)
        {
            StartupJob.DoJob();
        }
    }
    else
    {
        if (StartupJobs.Num() > 0)
        {
            float TotalJobValue = 0.f;

            for (const FNyotaAssetManagerStartupJob &StartupJob : StartupJobs)
            {
                TotalJobValue += StartupJob.JobWeight;
            }

            float AccumulatedJobValue = 0.0f;
            for (FNyotaAssetManagerStartupJob &StartupJob : StartupJobs)
            {
                const float JobValue = StartupJob.JobWeight;
                StartupJob.SubstepProgressDelegate.BindLambda(
                    [This = this, AccumulatedJobValue, JobValue, TotalJobValue](float NewProgress) {
                        const float SubstepAdjustment = FMath::Clamp(NewProgress, 0.0f, 1.0f) * JobValue;
                        const float OverallPercentWithSubstep =
                            (AccumulatedJobValue + SubstepAdjustment) / TotalJobValue;

                        This->UpdateInitialGameContentLoadPercent(OverallPercentWithSubstep);
                    }
                );

                StartupJob.DoJob();

                StartupJob.SubstepProgressDelegate.Unbind();

                AccumulatedJobValue += JobValue;

                UpdateInitialGameContentLoadPercent(AccumulatedJobValue / TotalJobValue);
            }
        }
        else
        {
            UpdateInitialGameContentLoadPercent(1.0f);
        }
    }

    StartupJobs.Empty();

    UE_LOG(
        LogTemp,
        Display,
        TEXT("All startup jobs took %.2f seconds to complete"),
        FPlatformTime::Seconds() - AllStartupJobsStartTime
    );
}

void UNyotaAssetManager::InitializeGameplayCueManager()
{
    SCOPED_BOOT_TIMING("UNyotaAssetManager::InitializeGameplayCueManager");

    UNyotaGameplayCueManager *GCM = UNyotaGameplayCueManager::Get();
    if (!GCM)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "GlobalGameplayCueManagerClass is not set to NyotaGameplayCueManager in "
                "DefaultGame.ini. GameplayCue preloading skipped."
            )
        );
        return;
    }
    GCM->LoadAlwaysLoadedCues();
}

void UNyotaAssetManager::UpdateInitialGameContentLoadPercent(float GameContentPercent)
{
}
