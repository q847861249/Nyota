// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "NyotaInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FNyotaInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UInputAction> InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
    FGameplayTag InputTag;
};

class UInputAction;
/**
 *
 */
UCLASS(BlueprintType, Const)
class NYOTA_API UNyotaInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    UNyotaInputConfig(const FObjectInitializer &ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Nyota | Pawn")
    const UInputAction *FindNativeInputActionForTag(const FGameplayTag &InputTag, bool bLogNotFound = true) const;

    UFUNCTION(BlueprintCallable, Category = "Nyota | Pawn")
    const UInputAction *FindAbilityInputActionForTag(const FGameplayTag &InputTag, bool bLogNotFound = true) const;

    // 所有者使用的输入操作列表。这些输入操作映射到游戏标签，必须手动绑定。
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
    TArray<FNyotaInputAction> NativeInputActions;

    // 所有者使用的输入操作列表。这些输入操作映射到游戏标签，并自动绑定到具有匹配输入标签的技能。
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
    TArray<FNyotaInputAction> AbilityInputActions;
};
