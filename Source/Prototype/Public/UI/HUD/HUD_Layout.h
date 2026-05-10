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
class PROTOTYPE_API UHUD_Layout : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	/** The lowest layer stack, typically used for persistent gameplay UI elements. */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> GameStack;

	/** The middle layer stack, used for menus, inventory, and pause screens. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;

	/** The highest layer stack, reserved for modal popups and critical notifications. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> ModalStack;

	/** MainUI is the parent widget element within GameStack. As the parent container, 
	 * it contains all the child widget elements to be displayed within GameStack.
	*/
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UCommonActivatableWidget>  MainUIClass;

	/** Storage MainUI reference when it be create and push to GameStack */
	UPROPERTY()
	TObjectPtr<UMainUI> MainUI;
protected:
	virtual void NativeConstruct() override;
};
