// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Player/PrimaryAbility.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Ability/BaseAbility.h"
#include "Tags/Nyota_Tag.h"
#include "AttributeSet/Nyota_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"

void UPrimaryAbility::PrimaryAttack()
{

	// Set up collision query parameters
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
    FCollisionQueryParams Params;
    Params.AddIgnoredActors(ActorsToIgnore);

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Block);
    
    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

    //Caculate HitBoxLocation
    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
    const FVector HitBoxLocation =  GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + FVector(0.f, 0.f, HitBoxElevationOffset);

    GetWorld()->OverlapMultiByChannel(OverlapResults,HitBoxLocation,FQuat::Identity,ECC_Pawn,Sphere,Params,ResponseParams);

    // Filter Hit Actor. Ignore the actor who has "Player" Tag.
    TArray<AActor*> FilterActor;
    for(const FOverlapResult& Result : OverlapResults)
    {
        // if(!IsValid(Result.GetActor())) continue;
        AActor* HitActor = Result.GetActor();
        if(!HitActor) continue;
        FilterActor.Add(HitActor);
        // In the future if the primary want to use complex logic we can use tag for help 
        // if (HitActor && !HitActor->ActorHasTag(FName("Player")))
        // {
        //     FilterActor.Add(HitActor);
        // }
        // if (HitActor) FilterActor.Add(HitActor);
    
    }

    SendHitReactEventToActor(FilterActor);
    
    
    if(bDrawDebug)
    {
        DrawHitBoxOverlapDebugs(FilterActor,HitBoxLocation);
    }
}

void UPrimaryAbility::DrawHitBoxOverlapDebugs(const TArray<AActor*>& FilterActor, const FVector &HitBoxLocation)
{
    DrawDebugSphere(GetWorld(),HitBoxLocation,HitBoxRadius,12,FColor::Red,false,2.f);
    for(const AActor* HitActor: FilterActor)
    {
        FVector DebugLocation = HitActor->GetActorLocation();
        DebugLocation.Z += 100.f;
        DrawDebugSphere(GetWorld(),DebugLocation,30.f,10,FColor::Green,false,2.f);
    }
}

void UPrimaryAbility::SendHitReactEventToActor(TArray<AActor*> HitActors)
{
    for(AActor* HitActor : HitActors)
    {
        if(!IsValid(HitActor)) continue;
        // Payload will contain some detail data of this event.
        FGameplayEventData Payload;
        Payload.Instigator = GetAvatarActorFromActorInfo();
        Payload.Target = HitActor;
        ABaseCharacter* BC = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
        if(BC) 
        {
            UAbilitySystemComponent* ASC = BC->GetAbilitySystemComponent();
            if(ASC)
            {
                //In the future, we can use a function to caculate the damage.
                Payload.EventMagnitude = -(ASC->GetNumericAttribute(UNyota_AttributeSet::GetAttackAttribute()));
            } 
        }
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor,Nyota::Ability::HitReact, Payload);
    }
}