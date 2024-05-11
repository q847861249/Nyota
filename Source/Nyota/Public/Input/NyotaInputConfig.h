// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "InputAction.h"
#include "GameplayTagContainer.h"

#include "NyotaInputConfig.generated.h"


USTRUCT(Blueprintable)
struct FInput {
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagName;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> Input;

};


UCLASS()
class NYOTA_API UNyotaInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInput> PlayerInput;

};
