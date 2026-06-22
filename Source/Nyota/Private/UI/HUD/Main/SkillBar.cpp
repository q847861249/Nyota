// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/SkillBar.h"
#include "Components/Image.h"
#include "Character/BasePlayer.h"
#include "DataAsset/PlayerCharacterDataAsset.h"
#include "UI/HUD/Main/SkillTooltip.h"
#include "Abilities/GameplayAbility.h"
#include "CommonBorder.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "Components/HorizontalBox.h"
#include "UI/HUD/Main/SkillSlot.h"
void USkillBar::Init(ABasePlayer* BasePlayer)
{
    if (!TooltipClass) return;
    if(!SkillSlotClass) return;

    UPlayerCharacterDataAsset* DataAsset = BasePlayer->GetConfig();
    if(!DataAsset) return;

    ASC = BasePlayer->GetAbilitySystemComponent();
    if(!ASC) return;

    for(FSkillUIData SkillData: DataAsset->Skills)
    {
        USkillTooltip* Tooltip = CreateWidget<USkillTooltip>( GetOwningPlayer(), TooltipClass ); 
        if (!Tooltip) continue;
        Tooltip->InitTooltip(SkillData.AbilityName,SkillData.AbilityDesc);
        USkillSlot* SkillSlot = CreateWidget<USkillSlot>( GetOwningPlayer(), SkillSlotClass );
        if(!SkillSlot) continue;
        SkillSlot->Init(ASC,SkillData);
        SkillSlot->Icon->SetToolTip(Tooltip);
        AbilityUIBar->AddChild(SkillSlot);
    }

}


