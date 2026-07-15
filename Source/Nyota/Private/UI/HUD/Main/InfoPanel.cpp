// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/InfoPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Engine/Texture2D.h"
#include "Character/BasePlayer.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "DataAsset/PlayerCharacterDataAsset.h"

void UInfoPanel::SetAvatar(UTexture2D* AvatarTexture)
{
    Avatar->SetBrushFromTexture(AvatarTexture);
}

void UInfoPanel::SetCoinBar(float Percent)
{
    CoinBar->SetPercent(Percent);
}

void UInfoPanel::Init(ABasePlayer* BasePlayer)
{
    UAbilitySystemComponent* ASC = BasePlayer->GetAbilitySystemComponent();
    if (!ASC) return;
    AttSet = Cast<UBaseAttributeSet>(BasePlayer->GetAttributeSet());
    //初始化当前的金币进度条
    const float CurrentCoin = AttSet->GetCoin();
    const float MaxCoin = AttSet->GetMaxCoin();
    SetCoinBar(CurrentCoin / MaxCoin);
    //绑定金币变化的回调函数
    ASC->GetGameplayAttributeValueChangeDelegate(AttSet->GetCoinAttribute()).AddUObject(this,&UInfoPanel::OnCoinChanged);

    UPlayerCharacterDataAsset* DataAsset = BasePlayer->GetConfig();
    if(!DataAsset) return;
    SetAvatar(DataAsset->Avatar);
}

void UInfoPanel::OnCoinChanged(const FOnAttributeChangeData& Data)
{
    const float CurrentCoin = Data.NewValue;
    const float MaxCoin = AttSet->GetMaxCoin();
    SetCoinBar(CurrentCoin / MaxCoin);
}