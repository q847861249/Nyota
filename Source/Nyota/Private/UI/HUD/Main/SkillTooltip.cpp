// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/SkillTooltip.h"
#include "Components/TextBlock.h"

void USkillTooltip::InitTooltip(const FText& SkillName, const FText& SkillDesc)
{
    if(SkillNameText)
    {
       SkillNameText->SetText(SkillName); 
    }
    else
    {
        UE_LOG(LogTemp,Warning,TEXT("SkillNameText空的"));
    }
    
    if(SkillDescText)
    {
        SkillDescText->SetText(SkillDesc);
    }
    else
    {
        UE_LOG(LogTemp,Warning,TEXT("SkillDescText空的"));
    }
    
}
