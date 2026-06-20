// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SkillBar.generated.h"

/**
 * 
 */
class UImage;
class ABasePlayer;
class USkillTooltip;
UCLASS()
class NYOTA_API USkillBar : public UCommonUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PassiveImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Q_Image;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> E_Image;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> R_Image;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillTooltip> TooltipClass;
public:
	void Init(ABasePlayer* BasePlayer);
};
