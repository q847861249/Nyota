// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h" 
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PrimaryAnimNotify.generated.h"

/**
 * This class is used to send event to notify actor montage already play in correct time.
 */
UCLASS()
class NYOTA_API UPrimaryAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NotifyTag;
};
