// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/NyotaAssetManager.h"
#include "./Input/NyotaGameplayTags.h"

UNyotaAssetManager::UNyotaAssetManager()
{
}

UNyotaAssetManager& UNyotaAssetManager::Get()
{
	check(GEngine);

	UNyotaAssetManager* MyAssetManager = Cast<UNyotaAssetManager>(GEngine->AssetManager);

	return  *MyAssetManager;
}

void UNyotaAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FNyotaGameplayTags::InitializeNativeTags();

}
