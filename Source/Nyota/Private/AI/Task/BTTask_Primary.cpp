// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Primary.h"
#include "AIController.h"
#include "AttributeSet/Nyota_AttributeSet.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Characters/Player/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "Tags/Nyota_Tag.h"
#include "AttributeSet/Nyota_AttributeSet.h"


UBTTask_Primary::UBTTask_Primary()
{
    NodeName = "Primary";

}

EBTNodeResult::Type UBTTask_Primary::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AEnemyCharacter* EC = Cast<AEnemyCharacter>(AIController->GetPawn());
    if(!EC) return EBTNodeResult::Failed;

    UBlackboardComponent* BC = AIController->GetBlackboardComponent();
    if(!BC) return EBTNodeResult::Failed; 

    APlayerCharacter* Target = Cast<APlayerCharacter>(BC->GetValueAsObject(GetSelectedBlackboardKey()));
    if(Target)
    {
        if(IsInAttackRange(EC,Target))
        {
            //Attack
            RotateToTarget(EC,Target);
            Attack(EC);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}

bool UBTTask_Primary::IsInAttackRange(AEnemyCharacter* OwnerCharacter,APlayerCharacter* Target)
{
    // Set up collision query parameters
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerCharacter);
    FCollisionQueryParams Params;
    Params.AddIgnoredActors(ActorsToIgnore);

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Block);
    
    float AttackRange = OwnerCharacter->GetAbilitySystemComponent()->GetNumericAttribute(UNyota_AttributeSet::GetAttackRangeAttribute());

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRange);

    GetWorld()->OverlapMultiByChannel(OverlapResults,OwnerCharacter->GetActorLocation(),FQuat::Identity,ECC_Pawn,Sphere,Params,ResponseParams);

    // Filter Hit Actor. Ignore the actor who has "Player" Tag.
    for(const FOverlapResult& Result : OverlapResults)
    {
        if(!IsValid(Result.GetActor())) continue;
        APlayerCharacter* FindActor = Cast<APlayerCharacter>(Result.GetActor());
        if(FindActor == Target) return true;
    }
    
    return false;

}

void UBTTask_Primary::RotateToTarget(AEnemyCharacter* OwnerCharacter, APlayerCharacter* Target)
{

    FVector LookAtVector = Target->GetActorLocation() - OwnerCharacter->GetActorLocation();
    LookAtVector.Z = 0; 

    if (LookAtVector.IsNearlyZero()) return;


    FRotator LookAtRotation = LookAtVector.Rotation();

    float OffsetYaw = -90.f; 
    LookAtRotation.Yaw += OffsetYaw;

    FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
    FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 25.f);
    OwnerCharacter->SetActorRotation(SmoothedRotation);
}

void UBTTask_Primary::Attack(AEnemyCharacter* EC)
{
    UAbilitySystemComponent* ASC = EC->GetAbilitySystemComponent();
    if(!ASC) return;
    FGameplayTagContainer TagContainer;
    TagContainer.AddTag(Nyota::Ability::Primary);
    ASC->TryActivateAbilitiesByTag(TagContainer);
}