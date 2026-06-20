// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerCharacterDataAsset.generated.h"

class UTexture2D;
UCLASS()
class NYOTA_API UPlayerCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTexture2D> Avatar;

	UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTexture2D> PassiveIcon;

	UPROPERTY(EditDefaultsOnly)
	FText PassiveName;	

	UPROPERTY(EditDefaultsOnly)
	FText PassiveDesc;	

	UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTexture2D> Q_Icon;

	UPROPERTY(EditDefaultsOnly)
	FText Q_Name;	

	UPROPERTY(EditDefaultsOnly)
	FText Q_Desc;	

	UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTexture2D> E_Icon;

	UPROPERTY(EditDefaultsOnly)
	FText E_Name;	

	UPROPERTY(EditDefaultsOnly)
	FText E_Desc;

	UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTexture2D> R_Icon;

	UPROPERTY(EditDefaultsOnly)
	FText R_Name;	

	UPROPERTY(EditDefaultsOnly)
	FText R_Desc;


};
