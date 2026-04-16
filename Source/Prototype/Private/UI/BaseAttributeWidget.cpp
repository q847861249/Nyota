// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseAttributeWidget.h"

bool UBaseAttributeWidget::MatchAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute> &Pair) const
{
    return Attribute == Pair.Key && MaxAttribute == Pair.Value;
}

void UBaseAttributeWidget::OnAttributeChange(
    const TTuple<FGameplayAttribute, FGameplayAttribute> &Pair, UBaseAttributeSet *BaseAttributeSet
)
{
    const float AttributeValue = Pair.Key.GetNumericValue(BaseAttributeSet);
    const float MaxAttributeValue = Pair.Value.GetNumericValue(BaseAttributeSet);

    BP_OnAttributeChanged(AttributeValue, MaxAttributeValue);
}