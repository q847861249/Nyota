// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/NyotaInputConfig.h"
#include "NyotaEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UNyotaEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserCalss, typename FunType>
	void BindNativeActionWithTag(const UNyotaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserCalss* Object, FunType Func);


};

template<class UserCalss, typename FunType>
void UNyotaEnhancedInputComponent::BindNativeActionWithTag(const UNyotaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserCalss* Object, FunType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}