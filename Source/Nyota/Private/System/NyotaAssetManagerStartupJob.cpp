// Fill out your copyright notice in the Description page of Project Settings.

#include "System/NyotaAssetManagerStartupJob.h"

TSharedPtr<FStreamableHandle> FNyotaAssetManagerStartupJob::DoJob() const
{
    const double JobStartTime = FPlatformTime::Seconds();

    TSharedPtr<FStreamableHandle> Handle;
    UE_LOG(LogTemp, Display, TEXT("Startup job \"%s\" starting"), *JobName);
    JobFunc(*this, Handle);

    if (Handle.IsValid())
    {
        Handle->BindUpdateDelegate(
            FStreamableUpdateDelegate::CreateRaw(
                this, &FNyotaAssetManagerStartupJob::UpdateSubstepProgressFromStreamable
            )
        );
        Handle->WaitUntilComplete(0.0f, false);
        Handle->BindUpdateDelegate(FStreamableUpdateDelegate());
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("Startup job \"%s\" took %.2f seconds to complete"),
        *JobName,
        FPlatformTime::Seconds() - JobStartTime
    );

    return Handle;
}
