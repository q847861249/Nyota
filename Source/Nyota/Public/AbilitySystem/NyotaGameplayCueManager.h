// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "NyotaGameplayCueManager.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UNyotaGameplayCueManager : public UGameplayCueManager
{
    GENERATED_BODY()

public:
    UNyotaGameplayCueManager(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    static UNyotaGameplayCueManager *Get();

    void LoadAlwaysLoadedCues();
};
