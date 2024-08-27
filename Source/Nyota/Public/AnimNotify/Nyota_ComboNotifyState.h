// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Nyota_ComboNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UNyota_ComboNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	bool bEndCombo = false;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;



	virtual FString GetEditorComment() override;
	virtual FString GetNotifyName_Implementation() const override;

private:
	// Used to check if the owner actor of this notify is the preview actor of Persona, in which case we don't do anything
	// to prevent log warning when getting components via Companion interfaces
	FString AnimationEditorPreviewActorString = "AnimationEditorPreviewActor";

	AActor* GetOwnerActor(USkeletalMeshComponent* MeshComponent) const;

};
