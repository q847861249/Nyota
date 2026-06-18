// Fill out your copyright notice in the Description page of Project Settings.

#include "System/NyotaGameData.h"

#include "System/NyotaAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NyotaGameData)

UNyotaGameData::UNyotaGameData()
{
}

const UNyotaGameData &UNyotaGameData::Get()
{
    return UNyotaAssetManager::Get().GetGameData();
}