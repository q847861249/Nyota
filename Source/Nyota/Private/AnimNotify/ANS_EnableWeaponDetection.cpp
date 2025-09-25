// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/ANS_EnableWeaponDetection.h"
#include "Character/NyotaCharacters.h"
#include "Ability/GA/GA_WeaponStartDetection.h"
#include "Weapon/NyotaWeapon.h"
#include "Debug/Debug.h"

void UANS_EnableWeaponDetection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp,Animation,TotalDuration,EventReference);

	if (ANyotaCharacters* NyotaCharacter = Cast<ANyotaCharacters>(MeshComp->GetOwner()))
	{
		if (ANyotaWeapon* weapon = NyotaCharacter->getWeapon())
		{
			weapon->WeaponCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

		}
	}
}

void UANS_EnableWeaponDetection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ANyotaCharacters* NyotaCharacter = Cast<ANyotaCharacters>(MeshComp->GetOwner()))
	{
		if (ANyotaWeapon* weapon = NyotaCharacter->getWeapon())
		{
			weapon->WeaponCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
	}
}


