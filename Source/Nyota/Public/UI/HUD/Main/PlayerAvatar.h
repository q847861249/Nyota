// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "PlayerAvatar.generated.h"

/**
 * 
 */
class UImage;
class UTexture2D;
UCLASS()
class NYOTA_API UPlayerAvatar : public UCommonUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AvatarImage;
public:
	void SetAvatar(UTexture2D* AvatarTexture);
};
