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
	// 存储找到的出生点
    UPROPERTY()
    TArray<AActor*> SpawnPoints;

    // 要生成的敌人类型（在编辑器里指定）
    UPROPERTY(EditAnywhere, Category = "Nyota|Spawning")
    TSubclassOf<class AEnemyCharacter> EnemyClass;

    // 延迟生成的时间
    UPROPERTY(EditAnywhere, Category = "Nyota|Spawning")
    float SpawnDelay = 0.1f;

    // 实际执行生成的函数
    void SpawnEnemiesAtPoints();



};
