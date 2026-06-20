// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/HUD_Layout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "UI/HUD/Main/MainUI.h"
#include "Character/BasePlayerController.h"
#include "Character/BasePlayer.h"
#include "UI/HUD/Main/InfoPanel.h"
#include "UI/HUD/Main/SkillBar.h"
void UHUD_Layout::NativeConstruct()
{
    Super::NativeConstruct();

    if(MainUIClass)
    {
        // Auto create MainUI instance and push to GameStack
        MainUI = Cast<UMainUI>(GameStack->AddWidget(MainUIClass));

        ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwningPlayer());
        if(!PC) return;
        MainUI->InfoPanel->Init(Cast<ABasePlayer>(PC->GetPawn()));
        MainUI->SkillBar->Init(Cast<ABasePlayer>(PC->GetPawn()));
    }
}
