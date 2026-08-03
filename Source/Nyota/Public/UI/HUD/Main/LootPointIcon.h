// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "LootPointIcon.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class UTexture2D;
UCLASS()
class NYOTA_API ULootPointIcon : public UCommonUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> LootPointIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> LootPointCountdownText;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointNotGenerate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointUpcomingUpdate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointComplete;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointRemaining;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointEnd;
};
