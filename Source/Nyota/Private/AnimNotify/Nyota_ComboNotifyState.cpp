// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Nyota_ComboNotifyState.h"
#include "Character/NyotaCharacters.h"
#include"Component/NyotaComponent.h"
#include"Debug/Debug.h"
#include"Character/NyotaMovementComponent.h"
#include "./Ability/GA/GA_Base.h"

void UNyota_ComboNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);

	const AActor* Owner = GetOwnerActor(MeshComp);
	if (!Owner)
	{
		return;
	}

	// run only on server
	if (!Owner->HasAuthority())
	{
		return;
	}

	UNyotaComponent* NyotaComponent = Owner->FindComponentByClass<UNyotaComponent>();
	if (NyotaComponent)
	{
		NyotaComponent->bComboWindowOpened = true;
	}


}

void UNyota_ComboNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

}

void UNyota_ComboNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

FString UNyota_ComboNotifyState::GetEditorComment()
{
	return TEXT("probably not yet implemented");
}

FString UNyota_ComboNotifyState::GetNotifyName_Implementation() const
{
	return bEndCombo ? "Nyota Combo Window (ending)     " : "Nyota Combo Window    ";

}

AActor* UNyota_ComboNotifyState::GetOwnerActor(USkeletalMeshComponent* MeshComponent) const
{
	AActor* OwnerActor = MeshComponent->GetOwner();
	if (!OwnerActor)
	{
		return nullptr;
	}
	return OwnerActor;
}
