// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API ULootDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loot")
	FName LootName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loot")
	TSubclassOf<AActor> LootActorClass;
};
