// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SkillTooltip.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class NYOTA_API USkillTooltip : public UCommonUserWidget
{
	GENERATED_BODY()
private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SkillNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SkillDescText;

public:
    void InitTooltip(const FText& SkillName, const FText& SkillDesc);
};
