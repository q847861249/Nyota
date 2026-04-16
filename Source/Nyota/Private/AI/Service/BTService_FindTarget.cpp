// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_FindTarget.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Controller/EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "AttributeSet/Nyota_AttributeSet.h"
#include "AbilitySystemComponent.h"
void UBTService_FindTarget::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AEnemyController* EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
    if(!EnemyController) return;
    AEnemyCharacter* EC = Cast<AEnemyCharacter>(EnemyController->GetPawn());
    if(!EC) return;
    UBlackboardComponent* BC = EnemyController->GetBlackboardComponent();
    if(!BC) return;

    // Set up collision query parameters
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(EnemyController->GetPawn());
    FCollisionQueryParams Params;
    Params.AddIgnoredActors(ActorsToIgnore);

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Block);
    
    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(BC->GetValueAsFloat(FName("DetectRadius")));

    GetWorld()->OverlapMultiByChannel(OverlapResults,EnemyController->GetPawn()->GetActorLocation(),FQuat::Identity,ECC_Pawn,Sphere,Params,ResponseParams);

    TArray<APlayerCharacter*> PlayerCharacterArr;
    for(const FOverlapResult& Result : OverlapResults)
    {
        if(!IsValid(Result.GetActor())) continue;
        APlayerCharacter* PlayerCharacterActor = Cast<APlayerCharacter>(Result.GetActor());
        if(PlayerCharacterActor) PlayerCharacterArr.Add(PlayerCharacterActor);
    
    }
    if (PlayerCharacterArr.Num() > 0) BC->SetValueAsObject(GetSelectedBlackboardKey(),PlayerCharacterArr[0]);

    if(BC->GetValueAsObject(GetSelectedBlackboardKey()))
    {
        APlayerCharacter* PC = Cast<APlayerCharacter>(BC->GetValueAsObject(GetSelectedBlackboardKey()));
        float Distance = FVector::Dist(PC->GetActorLocation(),EnemyController->GetPawn()->GetActorLocation());
        
        if(Distance>=BC->GetValueAsFloat(FName("LostRadius"))) BC->ClearValue(GetSelectedBlackboardKey());
    }

    DrawDebugSphere(GetWorld(),EnemyController->GetPawn()->GetActorLocation(),BC->GetValueAsFloat(FName("DetectRadius")),12,FColor::Red,false,2.f);
    DrawDebugSphere(GetWorld(),EnemyController->GetPawn()->GetActorLocation(),BC->GetValueAsFloat(FName("LostRadius")),12,FColor::Blue,false,1.0f);
    DrawDebugSphere(GetWorld(),EnemyController->GetPawn()->GetActorLocation(),
    EC->GetAbilitySystemComponent()->GetNumericAttribute(UNyota_AttributeSet::GetAttackRangeAttribute()),12,FColor::Yellow,false,0.5f);
}