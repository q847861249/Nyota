// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LootPoint.h"
#include "DataAsset/LootPointDataAsset.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "DrawDebugHelpers.h"
#include "Actor/Loot.h"
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
}

// Called every frame
void ALootPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsInit) return;

	FLinearColor CurrentColor = FLinearColor::Gray;
	switch (CurrentState)
	{
	case ELootPointState::CoolDown:
		CurrentColor = CoolDownColor;
		break;

	case ELootPointState::UpComing:
		CurrentColor = UpComingColor;
		break;

	case ELootPointState::Active:
		CurrentColor = ActiveColor;
		break;

	case ELootPointState::Remaining:
		CurrentColor = RemainingColor;
		break;

	case ELootPointState::Completed:
		CurrentColor = CompletedColor;
		break;
	}

	if (bDrawDebugRange)
	{
		DrawDebugCircle(
			GetWorld(),
			GetActorLocation(),
			Range,
			64,
			CurrentColor.ToFColor(true),
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
void ALootPoint::SetCurrentValue(int32 SubtractValue)
{
	CurrentValue -= (SubtractValue+1);
	if(CurrentValue <= TotalValue/2 ) SetRemainingState();
	if(CurrentValue == 0) SetCompleteState();
}

void ALootPoint::Init(float OutTime)
{
	IsInit = true;
	PrepareTime = OutTime;
	GetWorld()->GetTimerManager().SetTimer(PrepareTimerHandle,this,&ALootPoint::SetCoolDownState,PrepareTime,false);
}
void ALootPoint::SetCoolDownState()
{
	CurrentState = ELootPointState::CoolDown;
	GetWorld()->GetTimerManager().SetTimer(CoolDownTimerHandle,this,&ALootPoint::SetUpComingState,CoolDownTime,false);
}
void ALootPoint::SetUpComingState()
{
	CurrentState = ELootPointState::UpComing;
	GetWorld()->GetTimerManager().SetTimer(UpComingTimerHandle,this,&ALootPoint::SetActiveState,UpComingTime,false);
}
void ALootPoint::SetActiveState()
{
	CurrentState = ELootPointState::Active;
	GenerateLoot();
}
void ALootPoint::SetRemainingState()
{
	CurrentState = ELootPointState::Remaining;

}
void ALootPoint::SetCompleteState()
{
	CurrentState = ELootPointState::Completed;
	GetWorld()->GetTimerManager().SetTimer(CompletedTimerHandle,this,&ALootPoint::SetCoolDownState,5.f,false);
	TotalValue = 0;
}

void ALootPoint::GenerateLoot()
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
        for(FLootValueRate& Entry:LootConfig->ValueTable)
        {
            ValueThreshold += Entry.Percent;
            if(RandomValue <= ValueThreshold )
            {
                LootValue = Entry.LootValue;
                break;
            }
        }
        FLootDistributed LootArray = (LootConfig->LootTable)[LootValue];
        int32 RandomIndex = FMath::RandRange(0, (LootArray.LootItems).Num() - 1);
		TSubclassOf<ALoot> LootClass = (LootArray.LootItems)[RandomIndex];
		if(SpawnLootItem(LootClass))
		{
			TotalValue += LootValue + 1;
		}
    }
	CurrentValue = TotalValue;
}
bool ALootPoint::SpawnLootItem(TSubclassOf<ALoot> LootClass)
{
	UWorld* World = GetWorld();
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
	if (!NavSystem) return false;

	const FVector LootPointLocation = GetActorLocation();
	FNavLocation ProjectedCenter;
	//把一个世界坐标投影到一个最近的可导航点上
	const bool bProjected = NavSystem->ProjectPointToNavigation(LootPointLocation, ProjectedCenter, FVector(Range, Range, 5000.0f));
	if(!bProjected ) return false;
	//找到投影点附近一个随机的可导航点
	FNavLocation RandomNavLocation;
	const bool bFoundNavPoint = NavSystem->GetRandomPointInNavigableRadius( ProjectedCenter.Location,Range, RandomNavLocation );
	if (!bFoundNavPoint) return false;

	const FVector SpawnLocation = RandomNavLocation.Location + FVector(0.0f, 0.0f, 5.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;
	SpawnRotation.Yaw = FMath::FRandRange(0.0f, 360.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(LootClass,SpawnLocation,SpawnRotation,SpawnParams);
	if (!SpawnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Loot Actor failed"));
		return false;
	}
	ALoot* Loot = Cast<ALoot>(SpawnedActor);
	if(!Loot) return false;
    Loot->OnLootDestory.AddUObject(this,&ThisClass::SetCurrentValue);
	return true;
}
int32 ALootPoint::GetUpComingRemainingTime()
{
	if(CurrentState != ELootPointState::UpComing) return 0;
	return FMath::CeilToInt(GetWorld()->GetTimerManager().GetTimerRemaining(UpComingTimerHandle));
}

