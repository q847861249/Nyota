// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DeathAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/BaseCharacter.h"
void UDeathAbility::ApplyDeatn()
{
    ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if (!Character) return;

    UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
    if (MoveComp)
    {
        MoveComp->StopMovementImmediately();
        MoveComp->DisableMovement(); 
    }

    Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Character->SetLifeSpan(LifeSpan);
}
