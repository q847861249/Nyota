// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MainUI.generated.h"

class UInfoPanel;
class USkillBar;
UCLASS()
class NYOTA_API UMainUI : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInfoPanel> InfoPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillBar> SkillBar;
	
};
