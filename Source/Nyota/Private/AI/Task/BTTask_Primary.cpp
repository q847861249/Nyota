// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Primary.h"
#include "AIController.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Character/BaseEnemy.h"
#include "Character/BasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/GameTags.h"



UBTTask_Primary::UBTTask_Primary()
{
    NodeName = "Primary";

}

EBTNodeResult::Type UBTTask_Primary::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ABaseEnemy* EC = Cast<ABaseEnemy>(AIController->GetPawn());
    if(!EC) return EBTNodeResult::Failed;

    UBlackboardComponent* BC = AIController->GetBlackboardComponent();
    if(!BC) return EBTNodeResult::Failed; 

    ABasePlayer* Target = Cast<ABasePlayer>(BC->GetValueAsObject(GetSelectedBlackboardKey()));
    if(Target)
    {
        if(IsInAttackRange(EC,Target,BC))
        {
            //Rotate to Target and Attack
            RotateToTarget(EC,Target);
            Attack(EC);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}

bool UBTTask_Primary::IsInAttackRange(ABaseEnemy* OwnerCharacter,ABasePlayer* Target,UBlackboardComponent* BlackBoard)
{
    // Set up collision query parameters
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerCharacter);
    FCollisionQueryParams Params;
    Params.AddIgnoredActors(ActorsToIgnore);

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Block);
    
    float AttackRange = BlackBoard->GetValueAsFloat(FName("AttackRange"));
    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRange);

    GetWorld()->OverlapMultiByChannel(OverlapResults,OwnerCharacter->GetActorLocation(),FQuat::Identity,ECC_Pawn,Sphere,Params,ResponseParams);

    // Filter Hit Actor. Ignore the actor who has "Player" Tag.
    for(const FOverlapResult& Result : OverlapResults)
    {
        if(!IsValid(Result.GetActor())) continue;
        ABasePlayer* FindActor = Cast<ABasePlayer>(Result.GetActor());
        if(FindActor == Target) return true;
    }
    
    return false;

}

void UBTTask_Primary::RotateToTarget(ABaseEnemy* OwnerCharacter, ABasePlayer* Target)
{
    //Calculate the orientation vector
    FVector LookAtVector = Target->GetActorLocation() - OwnerCharacter->GetActorLocation();
    LookAtVector.Z = 0; 
    //if vector very small, ignore this change
    if (LookAtVector.IsNearlyZero()) return;
    //get rotator
    FRotator LookAtRotation = LookAtVector.Rotation();

    //caculate offset
    float OffsetYaw = 0.f; 
    LookAtRotation.Yaw += OffsetYaw;
    //Set new Rotation, use Interp function let rotate be smooth
    FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
    FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 25.f);
    OwnerCharacter->SetActorRotation(SmoothedRotation);
}

void UBTTask_Primary::Attack(ABaseEnemy* EC)
{
    UAbilitySystemComponent* ASC = EC->GetAbilitySystemComponent();
    if(!ASC) return;
    FGameplayTagContainer TagContainer;
    TagContainer.AddTag(Nyota::Ability_LightAttack);
    ASC->TryActivateAbilitiesByTag(TagContainer);
}
