// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootPoint.generated.h"

class ULootPointDataAsset;
class ULootDataAsset;
UCLASS()
class NYOTA_API ALootPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Init();
	void SpawnLootItem(ULootDataAsset* DataAsset);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULootPointDataAsset> LootConfig;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugRange = true;

	UPROPERTY(EditAnywhere)
	float Range = 50.0f;

	UPROPERTY(EditAnywhere)
	float Magnification = 1.0f;
};
