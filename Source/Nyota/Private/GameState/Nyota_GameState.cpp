// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/Nyota_GameState.h"
#include "Net/UnrealNetwork.h"
#include "Controller/Nyota_PlayerController.h"
#include "UI/HUD/Nyota_HUD.h"
#include "UI/HUD/HUD_Layout.h"
#include "UI/Game/Main/MainUI.h"
#include "UI/HUD/HUD_Layout.h"

void ANyota_GameState::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        RemainTime = TimeSetting;
        GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ANyota_GameState::UpdateTime, 1.0f, true);
    }

    ANyota_PlayerController* PC = Cast<ANyota_PlayerController>(GetWorld()->GetFirstPlayerController());
    if (!PC) return;
    ANyota_HUD* HUD = Cast<ANyota_HUD>(PC->GetHUD());
    if(!HUD) return;
    UHUD_Layout* HUD_layout = Cast<UHUD_Layout>(HUD->RootLayout);
    if(!HUD_layout) return;
    MainUI = HUD_layout->MainUI;
}

void ANyota_GameState::AddPlayerState(APlayerState* PlayerState)
{
    Super::AddPlayerState(PlayerState);
    UE_LOG(LogTemp,Warning,TEXT("加入"));
    if(MainUI)
    {
        MainUI->UpdatePlayerList(); 
        UE_LOG(LogTemp,Warning,TEXT("有"));
    }
    else
    {
        UE_LOG(LogTemp,Warning,TEXT("空的"));
    }

}
void ANyota_GameState::RemovePlayerState(APlayerState* PlayerState)
{
    Super::RemovePlayerState(PlayerState);
}

void ANyota_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION_NOTIFY(ANyota_GameState, RemainTime, COND_None, REPNOTIFY_Always);
}

void ANyota_GameState::OnRep_RemainTime()
{
    if(!MainUI) return;
    MainUI->UpdateRemainTime(RemainTime);
}

void ANyota_GameState::UpdateTime()
{
    if(RemainTime > 0)
    {
        RemainTime--;
    }
    else
    {
        UE_LOG(LogTemp,Warning,TEXT("GameOver"));
    }
}