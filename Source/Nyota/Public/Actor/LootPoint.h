// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootPoint.generated.h"

class ULootPointDataAsset;
class ULootDataAsset;
class ALoot;
UENUM(BlueprintType)
enum class ELootPointState : uint8
{
    CoolDown,
    UpComing,
    Active,
    Remaining,
	Completed
};
UCLASS()
class NYOTA_API ALootPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootPoint();
	void Init(float OutTime);
	int32 GetUpComingRemainingTime();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ELootPointState CurrentState = ELootPointState::CoolDown;

private:
	void SetCoolDownState();
	void SetUpComingState();
	void SetActiveState();
	void SetRemainingState();
	void SetCompleteState();
	void GenerateLoot();
	bool SpawnLootItem(TSubclassOf<ALoot> LootClass);
	UFUNCTION()
	void SetCurrentValue(int32 SubtractValue);	

	FTimerHandle PrepareTimerHandle;
	FTimerHandle CoolDownTimerHandle;
	FTimerHandle UpComingTimerHandle;
	FTimerHandle ActiveTimerHandle;
	FTimerHandle RemainingTimerHandle;
	FTimerHandle CompletedTimerHandle;

	UPROPERTY(EditAnywhere, Category="Debug")
	FLinearColor CoolDownColor = FLinearColor::Gray;

	UPROPERTY(EditAnywhere, Category="Debug")
	FLinearColor UpComingColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, Category="Debug")
	FLinearColor ActiveColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, Category="Debug")
	FLinearColor RemainingColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, Category="Debug")
	FLinearColor CompletedColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULootPointDataAsset> LootConfig;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugRange = true;

	UPROPERTY(EditAnywhere)
	float Range = 50.0f;

	UPROPERTY(EditAnywhere)
	float Magnification = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float UpComingTime = 10.f;

	//外部传递游戏开始的时候调用Coudldown的准备间隔时间
	float PrepareTime = 60.f;

	UPROPERTY(EditDefaultsOnly)
	float CoolDownTime = 60.f;

	bool IsInit = false;

	int32 TotalValue = 0;
	int32 CurrentValue = 0;
	

};
