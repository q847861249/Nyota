// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HealthBar.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/Nyota_AttributeSet.h"
#include "Components/ProgressBar.h"

void UHealthBar::InitialHealthBar(UAbilitySystemComponent* OutASC)
{
    ASC = OutASC;

    if (ASC)
    {
        //Using delegate to listen attribute change
        ASC->GetGameplayAttributeValueChangeDelegate(UNyota_AttributeSet::GetHealthAttribute())
           .AddUObject(this, &UHealthBar::OnHealthChanged);
        ASC->GetGameplayAttributeValueChangeDelegate(UNyota_AttributeSet::GetMaxHealthAttribute())
           .AddUObject(this, &UHealthBar::OnMaxHealthChanged);

        SetHealthBar();
    }
}

void UHealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
    SetHealthBar();
}

void UHealthBar::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
    SetHealthBar();
}

void UHealthBar::SetHealthBar()
{
    if(!HealthBar && !ASC) return;

    float CurrentMaxHealth = ASC->GetNumericAttribute(UNyota_AttributeSet::GetMaxHealthAttribute());
    float CurrentHealth = ASC->GetNumericAttribute(UNyota_AttributeSet::GetHealthAttribute());

    if (CurrentMaxHealth > 0.f)
    {
        HealthBar->SetPercent(CurrentHealth / CurrentMaxHealth);
    }
}
