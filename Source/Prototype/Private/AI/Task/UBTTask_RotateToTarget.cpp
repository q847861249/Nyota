// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/UBTTask_RotateToTarget.h"

EBTNodeResult::Type UUBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::Succeeded;
}

void UUBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}