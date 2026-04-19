// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Characters/Enemy/EnemyCharacter.h" 
#include "TimerManager.h"
// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 只在服务器执行生成逻辑
    if (!HasAuthority()) return;

    // 1. 寻找场景中所有的 TargetPoint
    // 你也可以通过给 TargetPoint 添加 Tag 来筛选特定的出生点
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), SpawnPoints);

    if (SpawnPoints.Num() > 0)
    {
        // 2. 设置定时器，延迟执行生成
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyManager::SpawnEnemiesAtPoints, SpawnDelay, false);
    }
}

void AEnemyManager::SpawnEnemiesAtPoints()
{
    if (!EnemyClass) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (AActor* Point : SpawnPoints)
    {
        if (Point)
        {
            // 在 TargetPoint 的位置生成敌人
            GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, Point->GetActorTransform(), SpawnParams);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Nyota Manager: 已在 %d 个出生点生成敌人"), SpawnPoints.Num());
}



