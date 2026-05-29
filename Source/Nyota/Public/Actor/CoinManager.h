// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinManager.generated.h"

class ACoin;
class ANyota_GameState;
UCLASS()
class NYOTA_API ACoinManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoinManager();

private:
	UPROPERTY(EditDefaultsOnly,Category="CoinSettings")
	int32 MaxCount = 7;

	UPROPERTY(EditDefaultsOnly,Category="CoinSettings")
	float SpawnRate = 1.0f;

	UPROPERTY(EditAnywhere,Category="CoinSettings")
	float SpawnRadius = 5000.0f;

	UPROPERTY(EditDefaultsOnly,Category="CoinSettings")
	TSubclassOf<ACoin> CoinClass;

	float CurrentSpawnRate = 0;

	int32 CurrentCount = 0;

	void SpawnCoin();

	UFUNCTION()
	void OnCoinDestroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
