// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/Main/PlayerListSection.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Characters/Player/PlayerCharacter.h"
#include "AttributeSet/Nyota_AttributeSet.h"
#include "GameplayEffectTypes.h"


void UPlayerListSection::NativeConstruct()
{
    Super::NativeConstruct();
}
void UPlayerListSection::NativeDestruct()
{
    Super::NativeDestruct();
    UnBind();
}
//this function will be called when player list section is constructed. Passing ASC, next will use asc to ini UI
void UPlayerListSection::SetASC(UAbilitySystemComponent *RepASC)
{
    ASC = RepASC;
    Inital();
}
//according the attribute set to initial UI
void UPlayerListSection::Inital()
{
    float CurrentMaxHealthValue = ASC->GetNumericAttribute(UNyota_AttributeSet::GetMaxHealthAttribute());
    float CurrentHealthValue = ASC->GetNumericAttribute(UNyota_AttributeSet::GetHealthAttribute());
    float CurrentMaxScore = ASC->GetNumericAttribute(UNyota_AttributeSet::GetMaxScoreAttribute());
    float CurrentScore = ASC->GetNumericAttribute(UNyota_AttributeSet::GetScoreAttribute());

    HealthBar->SetPercent(CurrentHealthValue/CurrentMaxHealthValue);
    ScoreBar->SetPercent(CurrentScore/CurrentMaxScore);

    ASC->GetGameplayAttributeValueChangeDelegate(UNyota_AttributeSet::GetHealthAttribute())
        .AddUObject(this, &UPlayerListSection::OnHealthValueChange);
    ASC->GetGameplayAttributeValueChangeDelegate(UNyota_AttributeSet::GetScoreAttribute())
        .AddUObject(this, &UPlayerListSection::OnScoreValueChange);
}

void UPlayerListSection::UnBind()
{
    if(!ASC) return;
    ASC->GetGameplayAttributeValueChangeDelegate(UNyota_AttributeSet::GetHealthAttribute()).RemoveAll(this);
    ASC->GetGameplayAttributeValueChangeDelegate(UNyota_AttributeSet::GetScoreAttribute()).RemoveAll(this);
}

void UPlayerListSection::OnHealthValueChange(const FOnAttributeChangeData& Data)
{
    if(!ASC) return;
    float CurrentMaxHealthValue = ASC->GetNumericAttribute(UNyota_AttributeSet::GetMaxHealthAttribute());
    HealthBar->SetPercent(Data.NewValue/CurrentMaxHealthValue);
}

void UPlayerListSection::OnScoreValueChange(const FOnAttributeChangeData& Data)
{
    if(!ASC) return;
    float CurrentMaxScore = ASC->GetNumericAttribute(UNyota_AttributeSet::GetMaxScoreAttribute());
    ScoreBar->SetPercent(Data.NewValue/CurrentMaxScore);
    UE_LOG(LogTemp,Warning,TEXT("Score:%f"),Data.NewValue);
}