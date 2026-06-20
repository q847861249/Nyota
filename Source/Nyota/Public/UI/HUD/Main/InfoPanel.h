// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InfoPanel.generated.h"

class UProgressBar;
class UImage;
class UTexture2D;
class ABasePlayer;
class UBaseAttributeSet;
struct FOnAttributeChangeData;

UCLASS()
class NYOTA_API UInfoPanel : public UCommonUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> CoinBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Avatar;

	TObjectPtr<UBaseAttributeSet> AttSet;

	void OnCoinChanged(const FOnAttributeChangeData& Data);

public:
	void SetAvatar(UTexture2D* AvatarTexture);
	void SetCoinBar(float Percent);
	void Init(ABasePlayer* BasePlayer);
};
