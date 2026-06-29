// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NyotaPawnData.generated.h"

class UNyotaAbilitySet;
class UNyotaInputConfig;
class UNyotaAbilityTagRelationshipMapping;
class UBaseAttributeSet;
/**
 *
 */
UCLASS(
    MinimalAPI, BlueprintType, Const,
    Meta = (DisplayName = "Nyota Pawn Data", ShortTooltip = "Data asset used to define a Pawn.")
)
class UNyotaPawnData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    NYOTA_API UNyotaPawnData(const FObjectInitializer &ObjectInitializer);

    // 用于实例化（创建）该 Pawn 的类。通常应该继承自 ABaseCharacter。
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Pawn")
    TSubclassOf<APawn> PawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Abilities")
    TArray<TObjectPtr<UNyotaAbilitySet>> AbilitySets;

    // 该 Pawn 执行 Action 时应使用哪套 Ability Tag 映射关系。
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Abilities")
    TObjectPtr<UNyotaAbilityTagRelationshipMapping> TagRelationshipMapping;

    // 玩家控制的角色使用的输入配置，用于创建输入映射和绑定输入动作。
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Input")
    TObjectPtr<UNyotaInputConfig> InputConfig;
};
