// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"

/**
 * @brief 负责从流式加载句柄（StreamableHandle）中汇报加载进度
 */
struct FNyotaAssetManagerStartupJob
{
    FNyotaAssetManagerStartupJob(
        const FString &InJobName,
        const TFunction<void(const FNyotaAssetManagerStartupJob &, TSharedPtr<FStreamableHandle> &)> &InJobFunc,
        float InJobWeight
    )
    {
    }
};
