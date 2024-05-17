// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventTrigger.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(EventLog, Log, All)

USTRUCT()
struct NYOTA_API FTimerTriggerStruct
{
	GENERATED_BODY()
public:
	double AddTriggerTimer;
	int TriggerTime;
	TFunction<void()>* TriggerEventCallable;

};

USTRUCT()
struct NYOTA_API FStatusTriggerStruct
{
	GENERATED_BODY()
public:
	bool TriggerStatus;
	bool IsLoop;
	bool IsRemove;
	TFunction<void()>* TriggerEventCallable;
};

UCLASS()
class NYOTA_API UEventTrigger : public UObject
{
	GENERATED_BODY()

public:
	UEventTrigger();

public:
	TArray<FTimerTriggerStruct*> TimerTriggerArray;
	TArray<FStatusTriggerStruct*> StatusTriggerArray;

public:

	 void Tick(float DeltaTime);

	 //根据信号状态触发事件
	 int AddSignalTrigger(TFunction<void()>* callback, bool IsHaveSingal, bool IsLoop);

	 void TriggerSignalEvent(int SignalEventID, bool IsRemove);

	 //在TriggerTime以后触发事件
	 void AddTimerTrigger(TFunction<void()>* callback, UWorld* World, double TriggerTime);
};
