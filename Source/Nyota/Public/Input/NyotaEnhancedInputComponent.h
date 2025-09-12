// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Input/NyotaInputConfig.h"
#include "NyotaInputConfig.h"
#include "InputAction.h"

#include "NyotaEnhancedInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class NYOTA_API UNyotaEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserCalss, typename FuncType>
	void BindActionByTag(const UNyotaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserCalss* Object, FuncType Func);

	template<class UserCalss, typename FuncType>
	void BindAbility(UNyotaInputConfig* InputConfig, UserCalss ContextObejct, FuncType PressedFunc, FuncType ReleasedFunc);

};

template<class UserCalss, typename FuncType>
void UNyotaEnhancedInputComponent::BindActionByTag(const UNyotaInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserCalss* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserCalss, typename FuncType>
inline void UNyotaEnhancedInputComponent::BindAbility(UNyotaInputConfig* InputConfig, UserCalss ContextObejct, FuncType PressedFunc, FuncType ReleasedFunc)
{
	check(InputConfig);


	for (FInputConfig& theInputConfig : InputConfig->getAbilityInputConfigs())
	{
		if (!theInputConfig.IsValid()) continue;

		BindAction(theInputConfig.Input, ETriggerEvent::Started, ContextObejct, PressedFunc, theInputConfig.TagName);
		BindAction(theInputConfig.Input, ETriggerEvent::Completed, ContextObejct, ReleasedFunc, theInputConfig.TagName);

	}

}




