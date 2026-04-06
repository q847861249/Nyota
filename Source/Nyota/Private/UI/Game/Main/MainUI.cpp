// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/Main/MainUI.h"
#include "Components/VerticalBox.h"
#include "UI/Game/Main/PlayerListSection.h"
#include "GameState/Nyota_GameState.h"
#include "Components/TextBlock.h"

void UMainUI::UpdatePlayerList()
{
    if (!PlayerList || !PlayerListClass) return;

    PlayerList->ClearChildren();

    ANyota_GameState* GS = Cast<ANyota_GameState>(GetWorld()->GetGameState());

    if (!GS) return;
    UE_LOG(LogTemp,Warning,TEXT("Array %d"),GS->PlayerArray.Num());

    for (APlayerState* PS : GS->PlayerArray)
    {
        if (PS)
        {
            UPlayerListSection* NewSection = CreateWidget<UPlayerListSection>(this, PlayerListClass);
            if (NewSection)
            {
                UE_LOG(LogTemp,Warning,TEXT("创建成功"));
                PlayerList->AddChildToVerticalBox(NewSection);
            }
        }
    }
}

void UMainUI::UpdateRemainTime(int32 NewTime)
{
    RemainTime->SetText(FText::AsNumber(NewTime));
}