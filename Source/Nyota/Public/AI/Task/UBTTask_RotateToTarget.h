// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UBTTask_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UUBTTask_RotateToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
