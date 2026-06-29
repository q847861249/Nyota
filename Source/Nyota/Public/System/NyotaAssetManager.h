// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "System/NyotaGameData.h"

#include "NyotaAssetManager.generated.h"

struct FNyotaAssetManagerStartupJob;
class UNyotaPawnData;

struct FNyotaBundles
{
    static const FName Equipped;
};

/**
 *
 */
UCLASS(Config = Game)
class NYOTA_API UNyotaAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    UNyotaAssetManager();

    /**
     *
     * @return 返回 AssetManager 单例对象。
     */
    static UNyotaAssetManager &Get();

    /**
     * @brief 获取 TSoftObjectPtr 引用的资产。如果资产尚未加载，将同步加载。
     * @tparam AssetType
     * @param AssetPointer
     * @param bKeepInMemory
     * @return
     */
    template <typename AssetType>
    static AssetType *GetAsset(const TSoftObjectPtr<AssetType> &AssetPointer, bool bKeepInMemory = true);

    /**
     * @brief 获取 TSoftClassPtr 引用的子类。如果资产尚未加载，将同步加载。
     * @tparam AssetType
     * @param AssetPointer
     * @param bKeepInMemory
     * @return
     */
    template <typename AssetType>
    static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType> &AssetPointer, bool bKeepInMemory = true);

    /**
     * @brief 输出当前已加载并由 AssetManager 追踪的所有资产。
     */
    static void DumpLoadedAssets();

    const UNyotaGameData &GetGameData();

    const UNyotaPawnData *GetDefaultPawnData() const;

protected:
    template <typename GameDataClass>
    const GameDataClass &GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass> &DataPath)
    {
        // 如果已经加载过，直接从缓存返回
        if (const TObjectPtr<UPrimaryDataAsset> *pResult = GameDataMap.Find(GameDataClass::StaticClass()))
        {
            return *CastChecked<GameDataClass>(*pResult);
        }

        // 否则执行同步阻塞加载
        return *CastChecked<const GameDataClass>(
            LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName())
        );
    }

    static UObject *SynchronousLoadAsset(const FSoftObjectPath &AssetPath);

    static bool ShouldLogAssetLoads();

    /**
     * @brief 以线程安全的方式将已加载的资产添加到常驻内存列表中。
     * @param Asset
     */
    void AddLoadedAsset(const UObject *Asset);

    virtual void StartInitialLoading() override;

#if WITH_EDITOR
    virtual void PreBeginPIE(bool bStartSimulate) override;
#endif

    UPrimaryDataAsset *LoadGameDataOfClass(
        TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset> &DataClassPath,
        FPrimaryAssetType PrimaryAssetType
    );

    // 项目全局游戏数据资产路径。
    UPROPERTY(Config)
    TSoftObjectPtr<UNyotaGameData> NyotaGameDataPath;

    // 已加载的游戏数据缓存。
    UPROPERTY(Transient)
    TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

    // 生成玩家 Pawn 时使用的默认 PawnData（当 PlayerState 上未设置时生效）
    UPROPERTY(Config)
    TSoftObjectPtr<UNyotaPawnData> DefaultPawnData;

private:
    /**
     * @brief 清空并执行 StartupJobs 数组中的所有启动任务。
     */
    void DoAllStartupJobs();

    /**
     * @brief初始化 Ability System
     */
    void InitializeGameplayCueManager();

    /**
     * @brief 在加载过程中周期性调用，可用于向加载界面推送加载进度。
     * @param GameContentPercent
     */
    void UpdateInitialGameContentLoadPercent(float GameContentPercent);

    // 启动时需要执行的任务列表，用于追踪启动进度。
    TArray<FNyotaAssetManagerStartupJob> StartupJobs;

    // AssetManager 已加载并追踪的资产集合。
    UPROPERTY()
    TSet<TObjectPtr<const UObject>> LoadedAssets;

    // 修改已加载资产列表时使用的临界区锁。
    FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType *UNyotaAssetManager::GetAsset(const TSoftObjectPtr<AssetType> &AssetPointer, bool bKeepInMemory)
{
    AssetType *LoadedAsset = nullptr;

    const FSoftObjectPath &AssetPath = AssetPointer.ToSoftObjectPath();

    if (AssetPath.IsValid())
    {
        LoadedAsset = AssetPointer.Get();
        if (!LoadedAsset)
        {
            LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));

            ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
        }

        if (LoadedAsset && bKeepInMemory)
        {
            Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
        }
    }

    return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UNyotaAssetManager::GetSubclass(const TSoftClassPtr<AssetType> &AssetPointer, bool bKeepInMemory)
{
    TSubclassOf<AssetType> LoadedSubclass;

    const FSoftObjectPath &AssetPath = AssetPointer.ToSoftObjectPath();

    if (AssetPath.IsValid())
    {
        LoadedSubclass = AssetPointer.Get();
        if (!LoadedSubclass)
        {
            LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));

            ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
        }

        if (LoadedSubclass && bKeepInMemory)
        {
            Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
        }
    }

    return LoadedSubclass;
}
