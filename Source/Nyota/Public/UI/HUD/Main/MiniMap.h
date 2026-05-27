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
	/**Level ground Left Top Position*/
	UPROPERTY(EditDefaultsOnly,Category = "MiniMapSetting")
	FVector2D LeftTopLocation;
	/**Level ground Right Buttom Position*/
	UPROPERTY(EditDefaultsOnly,Category = "MiniMapSetting")
	FVector2D RightButtomLocation;

	/**if there is some offset on minimap, use x/y offset variable to tweak*/
	UPROPERTY(EditAnyWhere,Category = "MiniMapSetting")
	float X_Location_offset = 0.f;
	UPROPERTY(EditAnyWhere,Category = "MiniMapSetting")
	float Y_Location_offset = 0.f;

	/**minimap display size */
	UPROPERTY(EditDefaultsOnly,Category = "MiniMapSetting")
	FVector2D ImageSize;

	/**minimap display image */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MinimalMapImage;

	/**pointer display image */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PointerImage;

	UPROPERTY()
    TWeakObjectPtr<APawn> CachedPlayerPawn;
};
