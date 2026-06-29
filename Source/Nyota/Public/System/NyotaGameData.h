// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NyotaGameData.generated.h"

class UGameplayEffect;
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

    // Gameplay effect used to apply damage.  Uses SetByCaller for the damage magnitude.
    UPROPERTY(
        EditDefaultsOnly, Category = "Default Gameplay Effects",
        meta = (DisplayName = "Damage Gameplay Effect (SetByCaller)")
    )
    TSoftClassPtr<UGameplayEffect> DamageGameplayEffect_SetByCaller;

    // Gameplay effect used to apply healing.  Uses SetByCaller for the healing magnitude.
    UPROPERTY(
        EditDefaultsOnly, Category = "Default Gameplay Effects",
        meta = (DisplayName = "Heal Gameplay Effect (SetByCaller)")
    )
    TSoftClassPtr<UGameplayEffect> HealGameplayEffect_SetByCaller;

    // Gameplay effect used to add and remove dynamic tags.
    UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
    TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
};
