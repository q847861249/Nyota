// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AEnemyController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;
public:

};
