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
enum class ELootPointState : uint8;
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
	TObjectPtr<UTexture2D> LootPointCoolDownTexture;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointUpcomingUpdateTexture;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointActiveTexture;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointCompletedTexture;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> LootPointRemainingTexture;
public:
	void SetTexture(ELootPointState CurrentState);
	void SetCountdownText(float newTime);
};
