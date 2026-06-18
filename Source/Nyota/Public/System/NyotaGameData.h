// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NyotaGameData.generated.h"

/**
 *
 */
UCLASS(
    BlueprintType, Const,
    meta = (DisplayName = "Nyota Game Data", ShortTooltip = "Data asset containing global game data.")
)
class NYOTA_API UNyotaGameData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UNyotaGameData();

    /**
     *
     * @return 获取已加载的游戏数据。
     */
    static const UNyotaGameData &Get();
};
