// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"

DECLARE_DELEGATE_OneParam(FNyotaAssetManagerStartupJobSubstepProgress, float);

/**
 * @brief 负责从流式加载句柄（StreamableHandle）中汇报加载进度
 */
struct FNyotaAssetManagerStartupJob
{
    FNyotaAssetManagerStartupJobSubstepProgress SubstepProgressDelegate;
    TFunction<void(const FNyotaAssetManagerStartupJob &, TSharedPtr<FStreamableHandle> &)> JobFunc;
    FString JobName;
    float JobWeight;
    mutable double LastUpdate = 0;

    FNyotaAssetManagerStartupJob(
        const FString &InJobName,
        const TFunction<void(const FNyotaAssetManagerStartupJob &, TSharedPtr<FStreamableHandle> &)> &InJobFunc,
        float InJobWeight
    )
        : JobFunc(InJobFunc), JobName(InJobName), JobWeight(InJobWeight)
    {
    }

    TSharedPtr<FStreamableHandle> DoJob() const;

    void UpdateSubstepProgress(float NewProgress) const
    {
        SubstepProgressDelegate.ExecuteIfBound(NewProgress);
    }

    void UpdateSubstepProgressFromStreamable(TSharedRef<FStreamableHandle> StreamableHandle) const
    {
        if (SubstepProgressDelegate.IsBound())
        {
            double Now = FPlatformTime::Seconds();
            if (LastUpdate - Now > 1.0 / 60)
            {
                SubstepProgressDelegate.Execute(StreamableHandle->GetProgress());
                LastUpdate = Now;
            }
        }
    }
};
