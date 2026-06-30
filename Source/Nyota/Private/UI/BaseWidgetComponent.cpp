// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgetComponent.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Character/BaseCharacter.h"
#include "AbilitySystem/NyotaAbilitySystemComponent.h"
#include "UI/BaseAttributeWidget.h"

void UBaseWidgetComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAbilitySystemData();

    if (!IsASCInitialized())
    {
        BaseCharacter->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);

        return;
    }

    InitializeAttributeDelegate();
}

void UBaseWidgetComponent::InitAbilitySystemData()
{
    BaseCharacter = Cast<ABaseCharacter>(GetOwner());
    BaseAttributeSet = Cast<UBaseAttributeSet>(BaseCharacter->GetAttributeSet());
    BaseAbilitySystemComponent = Cast<UNyotaAbilitySystemComponent>(BaseCharacter->GetAbilitySystemComponent());
}

bool UBaseWidgetComponent::IsASCInitialized() const
{
    return BaseAbilitySystemComponent.IsValid() && BaseAttributeSet.IsValid();
}

void UBaseWidgetComponent::InitializeAttributeDelegate()
{
    if (!BaseAttributeSet->bAttributesInitialized)
    {
        BaseAttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChange);
    }
    else
    {
        BindToAttributeChange();
    }
}

void UBaseWidgetComponent::BindWidgetToAttributeChange(
    UWidget *WidgetObj, const TTuple<FGameplayAttribute, FGameplayAttribute> &Pair
) const
{
    UBaseAttributeWidget *AttributeWidget = Cast<UBaseAttributeWidget>(WidgetObj);

    if (!AttributeWidget)
    {
        return;
    }

    if (!AttributeWidget->MatchAttributes(Pair))
    {
        return;
    }

    AttributeWidget->OnAttributeChange(Pair, BaseAttributeSet.Get());

    BaseAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda(
        [AttributeWidget, &Pair, this](const FOnAttributeChangeData &AttributeChangeData) {
            AttributeWidget->OnAttributeChange(Pair, BaseAttributeSet.Get());
        }
    );
}

void UBaseWidgetComponent::OnASCInitialized(UAbilitySystemComponent *ASC, UAttributeSet *AS)
{
    BaseAbilitySystemComponent = Cast<UNyotaAbilitySystemComponent>(ASC);
    BaseAttributeSet = Cast<UBaseAttributeSet>(AS);

    if (!IsASCInitialized())
    {
        return;
    }

    InitializeAttributeDelegate();
}

void UBaseWidgetComponent::BindToAttributeChange()
{
    for (const TTuple<FGameplayAttribute, FGameplayAttribute> &Pair : AttributeMap)
    {
        BindWidgetToAttributeChange(GetUserWidgetObject(), Pair);

        GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget *ChildWidget) {
            BindWidgetToAttributeChange(ChildWidget, Pair);
        });
    }
}