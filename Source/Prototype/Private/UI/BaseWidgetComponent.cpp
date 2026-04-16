// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidgetComponent.h"

#include "AttributeSet/BaseAttributeSet.h"
#include "Character/BaseCharacter.h"
#include "GA/CustomAbilitySystemComponent.h"

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
    BaseAbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(BaseCharacter->GetAbilitySystemComponent());
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

void UBaseWidgetComponent::OnASCInitialized(UAbilitySystemComponent *ASC, UAttributeSet *AS)
{
    BaseAbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(ASC);
    BaseAttributeSet = Cast<UBaseAttributeSet>(AS);
    
    if (!IsASCInitialized())
    {
        return;
    }
    
    InitializeAttributeDelegate();
}

void UBaseWidgetComponent::BindToAttributeChange()
{
}