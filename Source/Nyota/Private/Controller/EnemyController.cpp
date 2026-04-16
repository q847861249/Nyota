// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyController.h"



void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }
}