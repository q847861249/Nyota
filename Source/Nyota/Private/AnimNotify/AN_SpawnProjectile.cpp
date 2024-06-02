// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_SpawnProjectile.h"
#include "Debug/Debug.h"
#include"Character/NyotaCharacters.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Input/NyotaGameplayTags.h"


void UAN_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp->GetOwner()->HasAuthority()) {

		if (ANyotaCharacters* character = Cast<ANyotaCharacters>(MeshComp->GetOwner())) {
			
			character->SendGameEventByTag(FNyotaGameplayTags::Get().State_Shooting);

		}
	}
}
