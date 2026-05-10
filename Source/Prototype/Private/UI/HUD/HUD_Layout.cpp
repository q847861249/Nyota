// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/HUD_Layout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "UI/HUD/Main/MainUI.h"

void UHUD_Layout::NativeConstruct()
{
    Super::NativeConstruct();

    if(MainUIClass)
    {
        // Auto create MainUI instance and push to GameStack
        MainUI = Cast<UMainUI>(GameStack->AddWidget(MainUIClass));
    }
}
