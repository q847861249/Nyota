// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LootPoint.h"
#include "DataAsset/LootPointDataAsset.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALootPoint::ALootPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALootPoint::BeginPlay()
{
	Super::BeginPlay();
    Init();	
}

// Called every frame
void ALootPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebugRange)
	{
		DrawDebugCircle(
			GetWorld(),
			GetActorLocation(),
			Range,
			64,
			FColor::Green,
			false,
			0.0f,
			0,
			3.0f,
			FVector(1.0f, 0.0f, 0.0f),
			FVector(0.0f, 1.0f, 0.0f),
			false
		);
	}
}

void ALootPoint::Init()
{
	// caculate Loot number
    int32 LootNum = 0;
    float NumThreshold = 0.0f;
    float RandomNum = FMath::FRandRange(0.0f, 100.0f) * Magnification;
    if(!LootConfig) return;

    for(FLootNumberRate& Entry:LootConfig->NumTable)
    {
        NumThreshold += Entry.Percent;
        if(RandomNum <= NumThreshold )
        {
            LootNum = Entry.LootAmount;
            break;
        }
    }

    for(int32 i = 0; i < LootNum;i++)
    {
        // caculate Loot value
        int32 LootValue = 0;
        float ValueThreshold = 0.0f;
        float RandomValue = FMath::FRandRange(0.0f, 100.0f) * Magnification;
        // float RandomValue = FMath::RandRange(0, LootArray->LootItem.Num() - 1);

        for(FLootValueRate& Entry:LootConfig->ValueTable)
        {
            ValueThreshold += Entry.Percent;
            if(RandomValue <= ValueThreshold )
            {
                LootValue = Entry.LootValue;
                break;
            }
        }
		UE_LOG(LogTemp,Warning,TEXT("now the LootValue is %d"),LootValue);
        
        FLootDistributed LootArray = (LootConfig->LootTable)[LootValue];
        // int32 RandomIndex = FMath::FRandRange(0.0f,((LootArray.LootItem).Num()-1.0f));
        int32 RandomIndex = FMath::RandRange(0, (LootArray.LootItem).Num() - 1);
        ULootDataAsset* NewDataAsset = (LootArray.LootItem)[RandomIndex];
        SpawnLootItem(NewDataAsset);
    }
}

void ALootPoint::SpawnLootItem(ULootDataAsset* DataAsset)
{
	if (!DataAsset || !DataAsset->LootActorClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSystem is null"));
		return;
	}

	const FVector LootPointLocation = GetActorLocation();

	FNavLocation ProjectedCenter;

	const bool bProjected = NavSystem->ProjectPointToNavigation(
		LootPointLocation,
		ProjectedCenter,
		FVector(Range, Range, 5000.0f)
	);

	if (!bProjected)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("LootPoint cannot find nearby NavMesh. LootPointLocation: %s, Range: %f"),
			*LootPointLocation.ToString(),
			Range
		);
		return;
	}

	FNavLocation RandomNavLocation;

	const bool bFoundNavPoint = NavSystem->GetRandomPointInNavigableRadius(
		ProjectedCenter.Location,
		Range,
		RandomNavLocation
	);

	if (!bFoundNavPoint)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("No navigable point found. ProjectedCenter: %s, Range: %f"),
			*ProjectedCenter.Location.ToString(),
			Range
		);
		return;
	}

	const FVector TraceStart = RandomNavLocation.Location + FVector(0.0f, 0.0f, 1000.0f);
	const FVector TraceEnd = RandomNavLocation.Location - FVector(0.0f, 0.0f, 5000.0f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHitGround = World->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_WorldStatic,
		Params
	);

	if (!bHitGround)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nav point found, but ground trace failed. NavLocation: %s"),
			*RandomNavLocation.Location.ToString()
		);
		return;
	}

	const FVector SpawnLocation = Hit.ImpactPoint + FVector(0.0f, 0.0f, 5.0f);

	FRotator SpawnRotation = FRotator::ZeroRotator;
	SpawnRotation.Yaw = FMath::FRandRange(0.0f, 360.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(
		DataAsset->LootActorClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Loot Actor failed"));
	}


}

