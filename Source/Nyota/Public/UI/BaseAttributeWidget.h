// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSet/BaseAttributeSet.h"

#include "BaseAttributeWidget.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UBaseAttributeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    bool MatchAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute> &Pair) const;

    void OnAttributeChange(
        const TTuple<FGameplayAttribute, FGameplayAttribute> &Pair, UBaseAttributeSet *BaseAttributeSet
    );

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Attribute Change"))
    void BP_OnAttributeChanged(float NewValue, float NewMaxValue);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash | Attributes")
    FGameplayAttribute Attribute;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash | Attributes")
    FGameplayAttribute MaxAttribute;
};