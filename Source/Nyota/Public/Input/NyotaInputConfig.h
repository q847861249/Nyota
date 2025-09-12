// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "InputAction.h"
#include "GameplayTagContainer.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Component/Nyota_AbilitySystemComponent.h"

#include "NyotaInputConfig.generated.h"



USTRUCT(Blueprintable)
struct FInputConfig {
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (Categories = InputTag))
	FGameplayTag TagName;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> Input;

	bool IsValid() const 
	{
		return TagName.IsValid() && Input;
	}

};


UCLASS()
class NYOTA_API UNyotaInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:



	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;

	TArray<FInputConfig>& getAbilityInputConfigs() { return AbilityInputConfigs; };
	
	TArray<FInputConfig>& getInputConfigs() { return InputConfigs; };

	TObjectPtr<UInputMappingContext>& getDefaultMapContext() { return DefaultMappingContext; };

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (TitleProperty = "InputTag"))
	TArray<FInputConfig> InputConfigs;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (TitleProperty = "InputTag"))
	TArray<FInputConfig> AbilityInputConfigs;

};
