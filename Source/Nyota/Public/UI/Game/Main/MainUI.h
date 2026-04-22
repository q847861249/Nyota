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
	/** MainUI internal widget element. PlayerList secion's container */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> PlayerList;
	/** MainUI internal widget element. Be used to display remain time number  */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainTime;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UPlayerListSection> PlayerListSectionClass;


public:
	/**Depend on the number of player to update player list */
	void UpdatePlayerList();

	/** Be used to updated synchronously the remaining time from server on the screen */
	void UpdateRemainTime(int32 NewTime);
};
