// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SkillSlot.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class UGameplayAbility;
class UAbilitySystemComponent;
struct FSkillUIData;
UCLASS()
class NYOTA_API USkillSlot : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Mask;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CooldownText;

	TSubclassOf<UGameplayAbility> AbilityClass;

	TObjectPtr<UAbilitySystemComponent> ASC;

	float AbilityRemainTime;

	float AbilityDuration;

	void Init(UAbilitySystemComponent* OutASC,FSkillUIData SkillData);

	bool GetAbilityCoolDown();

	void UpdateCooldownUI();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
