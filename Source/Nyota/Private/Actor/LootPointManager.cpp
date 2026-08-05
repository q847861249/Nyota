// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LootPointManager.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/LootPoint.h"

// Sets default values
ALootPointManager::ALootPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALootPointManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ALootPoint::StaticClass(),LootPoinActors);
	for (AActor* Actor: LootPoinActors)
	{
		ALootPoint* LootPointActor = Cast<ALootPoint>(Actor);
		if(!IsValid(LootPointActor)) continue;
		UE_LOG(LogTemp,Warning,TEXT("发现LootPoint,正在初始化"));
		int32 currentValue = FMath::RandRange(1, RandowSeed * 2);
		UE_LOG(LogTemp,Warning,TEXT("the value is %d"),currentValue);
		LootPointActor->Init(currentValue);
	}
	
}

// Called every frame
void ALootPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

