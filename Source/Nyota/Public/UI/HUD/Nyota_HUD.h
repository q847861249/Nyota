// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Nyota_HUD.generated.h"

/**
 * 
 */
class UCommonUserWidget;
UCLASS()
class NYOTA_API ANyota_HUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCommonUserWidget> HUDLayoutClass;

	/**This value is used to storaged the HUD layout reference when it be created*/
	UPROPERTY()
	UCommonUserWidget* RootLayout;
protected:
	virtual void BeginPlay() override;

};
