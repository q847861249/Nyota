// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "OpenMap.generated.h"

/**
 * 
 */
class UCanvasPanel;
class ULootPointIcon;
class ALootPoint;
class UPlayerAvatar;
class UTexture2D;
UCLASS()
class NYOTA_API UOpenMap : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
    virtual void NativeOnActivated() override;
    virtual void NativeOnDeactivated() override;
	virtual void NativeTick(const FGeometry& MyGeometry,float InDeltaTime) override;
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> LootPointPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULootPointIcon> LootPointIconClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerAvatar> PlayerAvatarClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> DefaultPlayerAvatarTexture;

	UPROPERTY(EditDefaultsOnly,Category = "MapConfig")
	FVector2D LeftTopLocation;

	UPROPERTY(EditDefaultsOnly,Category = "MapConfig")
	FVector2D RightButtomLocation;

	UPROPERTY(EditDefaultsOnly, Category = "MapConfig")
	FVector2D ImageSize = FVector2D(48.f, 48.f);
	
	UPROPERTY()
	TMap<TObjectPtr<ALootPoint>, TObjectPtr<ULootPointIcon>> LootPointTMap;

	UPROPERTY()
	TObjectPtr<UPlayerAvatar> PlayerAvatar;

	void InitLootPoint();
	void InitPlayer();
	void RefreshLootPoint(const FVector2D& MapSize);
	void RefreshPlayer(const FVector2D& MapSize);

	FVector2D GetIconMapPosition(const FVector& TargetPosition,const FVector2D& MapSize);

};
