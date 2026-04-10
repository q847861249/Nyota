// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinDestroy);
UCLASS()
class NYOTA_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

private:
	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 10.f;
protected:
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	FOnCoinDestroy OnCoinDestroy;
	void Destroy();

};
