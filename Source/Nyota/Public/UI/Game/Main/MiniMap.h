// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Image.h"
#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MiniMap.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UMiniMap : public UCommonUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
public:
	//Left Top Position
	UPROPERTY(EditDefaultsOnly)
	FVector2D LeftTopLocation;
	//Right Buttom Position
	UPROPERTY(EditDefaultsOnly)
	FVector2D RightButtomLocation;

	UPROPERTY(EditAnyWhere,Category = "Offset")
	float X_Location_offset = 0.f;
	UPROPERTY(EditAnyWhere,Category = "Offset")
	float Y_Location_offset = 0.f;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ImageSize;

	// Minimal Map UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MinimalMapImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PointerImage;

	UPROPERTY()
    TWeakObjectPtr<APawn> CachedPlayerPawn;
};
