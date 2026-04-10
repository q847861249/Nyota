// Fill out your copyright notice in the Description page of Project Settings.


#include "GameItems/PickUpItem/Coin.h"


// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = LifeTime;
	bReplicates = true;
	bNetLoadOnClient = true;
}

void ACoin::Destroy() 
{
	SetLifeSpan(0.01f);
}


void ACoin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// if the Coin is Destoryed Broadcast delegate
	if(EndPlayReason == EEndPlayReason::Destroyed)
	{
		OnCoinDestroy.Broadcast();
	}
}