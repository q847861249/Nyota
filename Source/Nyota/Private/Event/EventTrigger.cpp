// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/EventTrigger.h"

DEFINE_LOG_CATEGORY(EventLog)

UEventTrigger::UEventTrigger()
{

}

// Called every frame
void UEventTrigger::Tick(float DeltaTime)
{

	//根据状态触发事件数组
	for (int i= 0; i < this->StatusTriggerArray.Num(); i++)
	{
		auto Trigger = this->StatusTriggerArray[i];
		if (Trigger->TriggerStatus)
		{
			if (Trigger->IsLoop)
			{
				(*Trigger->TriggerEventCallable)();
			}
			else
			{
				(*Trigger->TriggerEventCallable)();
				Trigger->TriggerStatus = false;
			}
			
		}

		if (Trigger->IsRemove)
		{
			this->StatusTriggerArray.Remove(Trigger);
		}
		
	};
	//指定时间触发事件数组
	for (int i=0; i < this->TimerTriggerArray.Num(); i++)
	{
		FTimerTriggerStruct* TimerTrigger = this->TimerTriggerArray[i];
		TimerTrigger->AddTriggerTimer += DeltaTime;
		if (TimerTrigger->AddTriggerTimer > TimerTrigger->TriggerTime)
		{
			if (TimerTrigger->TriggerEventCallable)
			{
				(*TimerTrigger->TriggerEventCallable)();
			}
			else
			{
				UE_LOG(EventLog, Error, TEXT("TimerTrigger%d Error"), TimerTrigger->TriggerTime);
			}
			
			//时间触发器调用一次需要从时间触发器中移除
			this->TimerTriggerArray.Remove(TimerTrigger);
		}	

	};

}

int UEventTrigger::AddSignalTrigger(TFunction<void()>* callback, bool IsHaveSingal, bool IsLoop)
{
	FStatusTriggerStruct* TimerTriggerStru = new FStatusTriggerStruct();
	TimerTriggerStru->TriggerEventCallable = callback;
	TimerTriggerStru->TriggerStatus = IsHaveSingal;
	TimerTriggerStru->IsLoop = false;
	this->StatusTriggerArray.Add(TimerTriggerStru);
	return this->StatusTriggerArray.Num() - 1;
}

void UEventTrigger::TriggerSignalEvent(int SignalEventID, bool IsRemove)
{
	this->StatusTriggerArray[SignalEventID]->TriggerStatus = true;
	this->StatusTriggerArray[SignalEventID]->IsRemove = IsRemove;

}

void UEventTrigger::AddTimerTrigger(TFunction<void()>* callback, UWorld* World, double TriggerTime)
{
	FTimerTriggerStruct* TimerTriggerStru = StaticCast<FTimerTriggerStruct*>(FMemory::Malloc(sizeof(FTimerTriggerStruct)));
	if (!TimerTriggerStru)
	{
		UE_LOG(EventLog, Error ,TEXT("UEventTrigger::AddTimerTrigger Eroor TriggerTime=%f"), TriggerTime);
		return;
	}
	TimerTriggerStru->TriggerTime = World->GetTimeSeconds() + TriggerTime;
	TimerTriggerStru->TriggerEventCallable = callback;
	TimerTriggerStru->AddTriggerTimer = World->GetTimeSeconds();
	this->TimerTriggerArray.Add(TimerTriggerStru);

}

