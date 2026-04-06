// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "HUD_Layout.generated.h"

/**
 * 
 */
class UCommonActivatableWidgetStack;
class UCommonActivatableWidget;
class UMainUI;
UCLASS()
class NYOTA_API UHUD_Layout : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> GameStack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> ModalStack;


	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UCommonActivatableWidget>  MainUIClass;

	UPROPERTY()
	TObjectPtr<UMainUI> MainUI;
protected:
	virtual void NativeConstruct() override;
};
