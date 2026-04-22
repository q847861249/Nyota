// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindTarget.generated.h"

/**
 * Using Collosion to check if find target, It will generate colliders within a certain radius centered on itself. 
 * If a collider collides with a player, it will consider the player detected.
 * Collision range come from Blackboard
 */
UCLASS()
class NYOTA_API UBTService_FindTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

protected:
	// UPROPERTY(EditDefaultsOnly)
	// bool bDrawDebug = true;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
