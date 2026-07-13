// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataAsset/LootDataAsset.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootPointDataAsset.generated.h"

class ULootDataAsset;
USTRUCT(BlueprintType)
struct FLootNumberRate
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float Percent; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LootAmount;  
};

USTRUCT(BlueprintType)
struct FLootValueRate
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float Percent; 

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 LootValue;  
};

USTRUCT(BlueprintType)
struct FLootDistributed
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "10"))
    int32 Value; 

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<TObjectPtr<ULootDataAsset>> LootItem;  
};
/**
 * 
 */
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
