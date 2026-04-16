// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Primary.generated.h"

class AEnemyCharacter;
class APlayerCharacter;
UCLASS()
class NYOTA_API UBTTask_Primary : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Primary();
private:
	bool IsInAttackRange(AEnemyCharacter* OwnerCharacter, APlayerCharacter* Target);
	void RotateToTarget(AEnemyCharacter* OwnerCharacter, APlayerCharacter* Target);
	void Attack(AEnemyCharacter* EC);
protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
