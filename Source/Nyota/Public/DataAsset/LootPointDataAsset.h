// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataAsset/LootDataAsset.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootPointDataAsset.generated.h"

class ULootDataAsset;
class ALoot;

/** @brief 定义一次采集掉落数量的抽取概率，供掉落点按累计权重选择数量。 */
USTRUCT(BlueprintType)
struct FLootNumberRate
{
    GENERATED_BODY()

    /** 数量档位的概率权重，范围 0～100；默认零避免未配置项参与随机抽取。 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float Percent = 0.0f;

    /** 该档位生成的掉落物数量，默认零表示尚未配置掉落数量。 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LootAmount = 0;
};

/** @brief 定义掉落价值档位的抽取概率，决定使用哪一组掉落物候选类。 */
USTRUCT(BlueprintType)
struct FLootValueRate
{
    GENERATED_BODY()

    /** 价值档位的概率权重，范围 0～100；零初始化确保反射结构具有确定默认值。 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float Percent = 0.0f;

    /** 掉落表的价值档位索引；默认选择第零档，实际范围由资产中的表长度决定。 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 LootValue = 0;
};

/** @brief 保存同一价值档位可生成的掉落物类，供掉落点随机选取。 */
USTRUCT(BlueprintType)
struct FLootDistributed
{
    GENERATED_BODY()

    /** 本组掉落物的价值档位标识，范围 0～10，默认零档。 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "10"))
    int32 Value = 0;

    /** 该档位的可生成 Actor 类列表，资产配置时应至少提供一个候选项。 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<TSubclassOf<ALoot>> LootItems;  
};
/** @brief 集中配置掉落数量、价值概率及候选物品，供关卡中的掉落点复用。 */
UCLASS()
class NYOTA_API ULootPointDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
    TArray<FLootNumberRate> NumTable; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
    TArray<FLootValueRate> ValueTable; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
    TArray<FLootDistributed> LootTable; 
};
