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

	//** 获取AssetManager **//
	static UNyotaAssetManager& Get();

protected:

	//** 初始化AssetManager **//
	virtual  void StartInitialLoading() override;


};
