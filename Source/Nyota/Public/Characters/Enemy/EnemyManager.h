// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class NYOTA_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/**Enemy Spawn point */
    UPROPERTY()
    TArray<AActor*> SpawnPoints;

    /**Enemy Spawn class */
    UPROPERTY(EditAnywhere, Category = "Nyota|Spawning")
    TSubclassOf<class AEnemyCharacter> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Nyota|Spawning")
    float SpawnDelay = 0.1f;

    /** Spawn function */
    void SpawnEnemiesAtPoints();



};
