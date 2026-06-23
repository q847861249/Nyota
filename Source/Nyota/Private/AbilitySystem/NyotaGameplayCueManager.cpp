// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/NyotaGameplayCueManager.h"

#include "AbilitySystemGlobals.h"

UNyotaGameplayCueManager::UNyotaGameplayCueManager(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UNyotaGameplayCueManager *UNyotaGameplayCueManager::Get()
{
    return Cast<UNyotaGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
}

void UNyotaGameplayCueManager::LoadAlwaysLoadedCues()
{
}