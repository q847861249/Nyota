// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
class UVerticalBox;
class UPlayerListSection;
class UTextBlock;
UCLASS()
class NYOTA_API UMainUI : public UCommonActivatableWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> PlayerList;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UPlayerListSection> PlayerListClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainTime;
public:
	void UpdatePlayerList();
	void UpdateRemainTime(int32 NewTime);
};
