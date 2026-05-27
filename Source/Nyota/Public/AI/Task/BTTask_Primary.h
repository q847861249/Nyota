// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Primary.generated.h"

class ABaseEnemy;
class ABasePlayer;
UCLASS()
class NYOTA_API UBTTask_Primary : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Primary();
private:
	/**
    * Checks if the target is within the acceptable attack range.
    * @param OwnerCharacter The enemy character performing the task.
    * @param Target Attack Target character
    * @return True if the target is within range, false otherwise.
    */
	bool IsInAttackRange(ABaseEnemy* OwnerCharacter, ABasePlayer* Target,UBlackboardComponent* BlackBoard);
	/**
    * Smoothly rotates the owner character to face the specified target.
    * @param OwnerCharacter The character that needs to rotate.
    * @param Target The target actor to look at.
    */
	void RotateToTarget(ABaseEnemy* OwnerCharacter, ABasePlayer* Target);
	
	void Attack(ABaseEnemy* EC);
protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	// UPROPERTY(EditDefaultsOnly)
	// float AttackRange = 25.0f;
};
