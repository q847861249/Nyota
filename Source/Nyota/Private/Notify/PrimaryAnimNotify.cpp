// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/PrimaryAnimNotify.h"
#include "AbilitySystemBlueprintLibrary.h"

void UPrimaryAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{

    FGameplayEventData Payload;
    Payload.Instigator = MeshComp->GetOwner();
    //send NotifyTag event to owner
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), NotifyTag, Payload);
}

