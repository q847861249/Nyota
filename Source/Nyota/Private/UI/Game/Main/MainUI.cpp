// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/Main/MainUI.h"
#include "Components/VerticalBox.h"
#include "UI/Game/Main/PlayerListSection.h"
#include "GameState/Nyota_GameState.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/Nyota_PlayerState.h"

void UMainUI::UpdatePlayerList()
{
    if (!PlayerList || !PlayerListClass) return;

    PlayerList->ClearChildren();

    ANyota_GameState* GS = Cast<ANyota_GameState>(GetWorld()->GetGameState());
    if (!GS) return;

    for (APlayerState* PS : GS->PlayerArray)
    {
        ANyota_PlayerState* Nyota_PS = Cast<ANyota_PlayerState>(PS);
        if (Nyota_PS)
        {
            UPlayerListSection* NewSection = CreateWidget<UPlayerListSection>(this, PlayerListClass);
            if (NewSection)
            {
                PlayerList->AddChildToVerticalBox(NewSection);
                UAbilitySystemComponent* ASC = Nyota_PS->GetAbilitySystemComponent();
                if (ASC) NewSection->SetASC(ASC);
                else UE_LOG(LogTemp,Warning,TEXT("没有ASC"));
            }
        }
    }
}

void UMainUI::UpdateRemainTime(int32 NewTime)
{
    RemainTime->SetText(FText::AsNumber(NewTime));
}