// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Nyota_HUD.h"
#include "CommonUserWidget.h"

void ANyota_HUD::BeginPlay()
{
    Super::BeginPlay();

    if(HUDLayoutClass)
    {
        //create HUD layout and add it to viewport
        RootLayout = CreateWidget<UCommonUserWidget>(GetOwningPlayerController(),HUDLayoutClass);
        RootLayout->AddToViewport();

    }
}
