// Fill out your copyright notice in the Description page of Project Settings.


#include "GameItems/PickUpItem/CoinManager.h"
#include "NavigationSystem.h"
#include "GameItems/PickUpItem/Coin.h"
#include "Components/CapsuleComponent.h"
#include "GameState/Nyota_GameState.h"
// Sets default values
ACoinManager::ACoinManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACoinManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACoinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// only server side to cotroll coin spawn
	if(HasAuthority())
	{
		CurrentSpawnRate += DeltaTime;
		if(CurrentSpawnRate >= SpawnRate)
		{
			SpawnCoin();
			CurrentSpawnRate = 0.f;
		}
	}

}

void ACoinManager::SpawnCoin()
{
	if(CurrentCount > MaxCount || !CoinClass) return;
	
	// Find a Random point on NavMesh 
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    FNavLocation RandomLocation;
	if (NavSys && NavSys->GetRandomReachablePointInRadius(GetActorLocation(), SpawnRadius, RandomLocation))
	{
		ACoin* NewCoin = GetWorld()->SpawnActor<ACoin>(CoinClass, RandomLocation.Location, FRotator::ZeroRotator);
		if (NewCoin)
        {
			// tweak the coin location. Prevent the coin sink into the ground
			UCapsuleComponent* Capsule = NewCoin->FindComponentByClass<UCapsuleComponent>();
			if (Capsule)
			{
				float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
				NewCoin->AddActorWorldOffset(FVector(0.f, 0.f, HalfHeight));
			}
			// Increase the number of currently alive coins.
            CurrentCount++;
			// when coin destroy minus the number of currently alive coins
            NewCoin->OnCoinDestroy.AddDynamic(this, &ACoinManager::OnCoinDestroy);
        }
	}
}

void ACoinManager::OnCoinDestroy()
{
	CurrentCount--;
}

