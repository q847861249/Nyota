// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loot.generated.h"
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLootDestory,int32);
UCLASS()
class NYOTA_API ALoot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoot();
	int32 GetLootValue();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Destory();
	FOnLootDestory OnLootDestory;
private:
	UPROPERTY(EditDefaultsOnly)
	int32 LootValue = 0;

};
