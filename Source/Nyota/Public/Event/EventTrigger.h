// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(EventLog, Log, All)

template<typename NotifyParamType>
struct NYOTA_API FTimerTriggerStruct
{

public:
	double AddTriggerTimer;
	int TriggerTime;
	TMulticastDelegate<void(NotifyParamType*)>* TriggerEventCallable;

};


template<typename NotifyParamType>
struct NYOTA_API FStatusTriggerStruct
{

public:
	bool TriggerStatus;
	bool IsLoop;
	bool IsRemove;
	TMulticastDelegate<void(NotifyParamType*)>* TriggerEventCallable;
};

//template<typename NotifyParamType>

template<typename NotifyParamType>
class NYOTA_API TOneParamDelegate : TDelegate<void(NotifyParamType*)>
{

private:


public:

};


template<typename NotifyParamType>
class NYOTA_API EventTrigger
{

public:
	EventTrigger();

public:
	TArray<FTimerTriggerStruct<NotifyParamType>*> TimerTriggerArray;
	TArray<FStatusTriggerStruct<NotifyParamType>*> StatusTriggerArray;

public:
	 //template<typename NotifyParamType>
	 void Tick(float DeltaTime)
	 {

		 //根据状态触发事件数组
		 if (this->StatusTriggerArray.Num() > 0)
		 {
			 for (int i = 0; i < this->StatusTriggerArray.Num(); i++)
			 {
				 auto Trigger = this->StatusTriggerArray[i];
				 if (Trigger->TriggerStatus)
				 {
					 FString S = FString("NULL");
					 if (Trigger->IsLoop)
					 {
						 
						 Trigger->TriggerEventCallable->Broadcast(&S);
					 }
					 else
					 {
						 Trigger->TriggerEventCallable->Broadcast(&S);
						 Trigger->TriggerStatus = false;
					 }

				 }

				 if (Trigger->IsRemove)
				 {
					 this->StatusTriggerArray.Remove(Trigger);
				 }

			 };
		 }

		 //指定时间触发事件数组
		 for (int i = 0; i < this->TimerTriggerArray.Num(); i++)
		 {
			 auto* TimerTrigger = this->TimerTriggerArray[i];
			 TimerTrigger->AddTriggerTimer += DeltaTime;
			 if (TimerTrigger->AddTriggerTimer > TimerTrigger->TriggerTime)
			 {
				 if (TimerTrigger->TriggerEventCallable)
				 {
					 //(*((TFunction<void()>*)TimerTrigger->TriggerEventCallable.Get()))();
					 FString S = FString("NULL");
					 TimerTrigger->TriggerEventCallable->Broadcast(&S);
					 //(TimerTrigger->TriggerEventCallable)();
				 }
				 else
				 {
					 UE_LOG(EventLog, Error, TEXT("TimerTrigger%d Error"), TimerTrigger->TriggerTime);
				 }

				 //时间触发器调用一次需要从时间触发器中移除
				 this->TimerTriggerArray.Remove(TimerTrigger);
				 //free(TimerTrigger->TriggerEventCallable.Get());
			 }

		 };

	 };

	 //根据信号状态触发事件
	 //template<typename NotifyParamType>
	 int AddSignalTrigger(TMulticastDelegate<void(NotifyParamType*)>* NotifyDelegate, bool IsHaveSingal, bool IsLoop)
	 {
		 FStatusTriggerStruct<NotifyParamType>* TimerTriggerStru = StaticCast<FStatusTriggerStruct<NotifyParamType>*>(FMemory::Malloc(sizeof(FStatusTriggerStruct<NotifyParamType>)));
		 TimerTriggerStru->TriggerEventCallable = NotifyDelegate;
		 TimerTriggerStru->TriggerStatus = IsHaveSingal;
		 TimerTriggerStru->IsLoop = false;
		 this->StatusTriggerArray.Add(TimerTriggerStru);
		 return this->StatusTriggerArray.Num() - 1;
	 };

	 void TriggerSignalEvent(int SignalEventID, bool IsRemove)
	 {
		 this->StatusTriggerArray[SignalEventID]->TriggerStatus = true;
		 this->StatusTriggerArray[SignalEventID]->IsRemove = IsRemove;

	 };

	 //在TriggerTime以后触发事件
	 //template<typename NotifyParamType>
	 void AddTimerTrigger(TMulticastDelegate<void(NotifyParamType*)>* NotifyDelegate, UWorld* World, double TriggerTime)
	 {
		 FTimerTriggerStruct<NotifyParamType>* TimerTriggerStru = StaticCast<FTimerTriggerStruct<NotifyParamType>*>(FMemory::Malloc(sizeof(FTimerTriggerStruct<NotifyParamType>)));
		 if (!TimerTriggerStru)
		 {
			 UE_LOG(EventLog, Error, TEXT("UEventTrigger::AddTimerTrigger Eroor TriggerTime=%f"), TriggerTime);
			 return;
		 }
		 TimerTriggerStru->TriggerTime = World->GetTimeSeconds() + TriggerTime;
		 TimerTriggerStru->TriggerEventCallable = NotifyDelegate;
		 TimerTriggerStru->AddTriggerTimer = World->GetTimeSeconds();
		 this->TimerTriggerArray.Add(TimerTriggerStru);

	 };

};


template<typename NotifyObjType1, typename NotifyObjType2>
class NYOTA_API TwoTypeEventTrigger
{
private:
	//EventTrigger<APawnBase> PawnBaseTypeEventTrigger;
	//EventTrigger<APawnBase> PawnBaseTypeEventTrigger

};
