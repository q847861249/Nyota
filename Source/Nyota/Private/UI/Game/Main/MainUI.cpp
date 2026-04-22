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
    if (!PlayerList || !PlayerListSectionClass) return;
    // when the playerlist container update, clear this container first.
    PlayerList->ClearChildren();
    //Get GameState, we will get current all playerState in the game from PlayerArray.
    //PlayerArray is an array maintained by the official UE team. And it will automically update when a new playerState add or remove.
    ANyota_GameState* GS = Cast<ANyota_GameState>(GetWorld()->GetGameState());
    if (!GS) return;
    // loop playerArray to get all playerState
    for (APlayerState* PS : GS->PlayerArray)
    {
        //each playerState will create a playerlist section(this is a UI Section) and add to playerList container.
        ANyota_PlayerState* Nyota_PS = Cast<ANyota_PlayerState>(PS);
        if (Nyota_PS)
        {
            UPlayerListSection* NewSection = CreateWidget<UPlayerListSection>(this, PlayerListSectionClass);
            if (NewSection)
            {
                PlayerList->AddChildToVerticalBox(NewSection);
                UAbilitySystemComponent* ASC = Nyota_PS->GetAbilitySystemComponent();
                //passing the ASC come from this PlayerState to PlayerListSection
                if (ASC) NewSection->SetASC(ASC);
                else UE_LOG(LogTemp,Warning,TEXT("没有ASC"));
            }
        }
    }
}
//Update RemainTime UI
void UMainUI::UpdateRemainTime(int32 NewTime)
{
    RemainTime->SetText(FText::AsNumber(NewTime));
}