// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AN_CombEnd.h"
#include "Character/NyotaCharacters.h"
#include "Component/NyotaComponent.h"
#include "Input/NyotaGameplayTags.h"

void UAN_CombEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp->GetOwner()->HasAuthority()) 
	{
		if (ANyotaCharacters* character = Cast<ANyotaCharacters>(MeshComp->GetOwner())) 
		{
			character->SendGameEventByTag(FNyotaGameplayTags::Get().State_ComboWindowEnd);
		}
	}
}
