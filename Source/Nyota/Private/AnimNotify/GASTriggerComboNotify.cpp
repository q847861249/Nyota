// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/GASTriggerComboNotify.h"
#include "Component/NyotaComponent.h"
#include "Character/NyotaCharacters.h"

void UGASTriggerComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	const ANyotaCharacters* Owner = Cast<ANyotaCharacters>(GetOwnerActor(MeshComp));
	if (!Owner)
	{
		return;
	}

	// run only on server
	if (!Owner->HasAuthority())
	{
		return;
	}

	UNyotaComponent* ComboManagerComponent = Cast<UNyotaComponent>(Owner->NyotaComponent);

	if (!ComboManagerComponent)
	{
		return;
	}

	ComboManagerComponent->bRequestTriggerCombo = true;

}

FString UGASTriggerComboNotify::GetNotifyName_Implementation() const
{
    return 	 "Nyota Trigger Combo";
}

AActor* UGASTriggerComboNotify::GetOwnerActor(USkeletalMeshComponent* MeshComponent) const
{
	AActor* OwnerActor = MeshComponent->GetOwner();
	if (!OwnerActor)
	{
		return nullptr;
	}

	const FString ActorName = OwnerActor->GetName();
	if (ActorName.StartsWith(AnimationEditorPreviewActorString))
	{
		return nullptr;
	}

	return OwnerActor;
}
