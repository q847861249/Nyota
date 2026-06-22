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
class UGameplayAbility;
class UCommonBorder;
class UTextBlock;
class UAbilitySystemComponent;
class UHorizontalBox;
class USkillSlot;
UCLASS()
class NYOTA_API USkillBar : public UCommonUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillTooltip> TooltipClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillSlot> SkillSlotClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> AbilityUIBar;

	TObjectPtr<UAbilitySystemComponent> ASC;

public:
	void Init(ABasePlayer* BasePlayer);
};
