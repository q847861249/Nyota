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
	
    if (!HasAuthority()) return;

    //Find spawn point at world
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), SpawnPoints);

    if (SpawnPoints.Num() > 0)
    {
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
            //spawn enemy
            GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, Point->GetActorTransform(), SpawnParams);
        }
    }
    
}



