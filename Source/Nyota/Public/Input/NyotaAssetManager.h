// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "NyotaAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UNyotaAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
	UNyotaAssetManager();

	//** ��ȡAssetManager **//
	static UNyotaAssetManager& Get();

protected:

	//** ��ʼ��AssetManager **//
	virtual  void StartInitialLoading() override;


};
