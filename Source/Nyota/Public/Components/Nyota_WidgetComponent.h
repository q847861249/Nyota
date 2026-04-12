// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Nyota_WidgetComponent.generated.h"


UCLASS()
class NYOTA_API UNyota_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
protected:
	virtual void InitWidget() override;
};
