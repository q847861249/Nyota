// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/LootPointIcon.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Actor/LootPoint.h"

void ULootPointIcon::SetTexture(ELootPointState CurrentState)
{
    switch (CurrentState)
    {
        case ELootPointState::CoolDown:
            if(!LootPointCoolDownTexture) break;
            LootPointIcon->SetBrushFromTexture(LootPointCoolDownTexture);
            break;
        case ELootPointState::UpComing:
            if(!LootPointUpcomingUpdateTexture) break;
            LootPointIcon->SetBrushFromTexture(LootPointUpcomingUpdateTexture);
            break;
        case ELootPointState::Active:
            if(!LootPointActiveTexture) break;
            LootPointIcon->SetBrushFromTexture(LootPointActiveTexture);
            break;
        case ELootPointState::Remaining:
            if(!LootPointRemainingTexture) break;
            LootPointIcon->SetBrushFromTexture(LootPointRemainingTexture);
            break;
        case ELootPointState::Completed:
            if(!LootPointCompletedTexture) break;
            LootPointIcon->SetBrushFromTexture(LootPointCompletedTexture);
            break;
        default:
            break;
    }
}
void ULootPointIcon::SetCountdownText(float newTime)
{
    if(newTime == 0)
    {
        LootPointCountdownText->SetText(FText::GetEmpty());
    }
    else
    {
        LootPointCountdownText->SetText(FText::AsNumber(newTime));
    }
}