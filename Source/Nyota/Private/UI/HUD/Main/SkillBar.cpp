// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/SkillBar.h"
#include "Components/Image.h"
#include "Character/BasePlayer.h"
#include "DataAsset/PlayerCharacterDataAsset.h"
#include "UI/HUD/Main/SkillTooltip.h"
void USkillBar::Init(ABasePlayer* BasePlayer)
{
    if (!TooltipClass) return;

    UPlayerCharacterDataAsset* DataAsset = BasePlayer->GetConfig();
    if(!DataAsset) return;

    //设置被动UI信息
    PassiveImage->SetBrushFromTexture(DataAsset->PassiveIcon);
    USkillTooltip* PassiveTooltip = CreateWidget<USkillTooltip>( GetOwningPlayer(), TooltipClass ); 
    if (!PassiveTooltip) return;
    PassiveTooltip->InitTooltip(DataAsset->PassiveName,DataAsset->PassiveDesc); 
    PassiveImage->SetToolTip(PassiveTooltip);

    //设置Q技能UI信息
    Q_Image->SetBrushFromTexture(DataAsset->Q_Icon);
    USkillTooltip* Q_Tooltip = CreateWidget<USkillTooltip>( GetOwningPlayer(), TooltipClass ); 
    if (!Q_Tooltip) return;
    Q_Tooltip->InitTooltip(DataAsset->Q_Name,DataAsset->Q_Desc); 
    Q_Image->SetToolTip(Q_Tooltip);

    //设置E技能UI信息
    E_Image->SetBrushFromTexture(DataAsset->E_Icon);
    USkillTooltip* E_Tooltip = CreateWidget<USkillTooltip>( GetOwningPlayer(), TooltipClass ); 
    if (!E_Tooltip) return;
    E_Tooltip->InitTooltip(DataAsset->E_Name,DataAsset->E_Desc); 
    E_Image->SetToolTip(E_Tooltip);
}