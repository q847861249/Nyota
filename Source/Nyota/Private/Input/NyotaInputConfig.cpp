// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/NyotaInputConfig.h"

const UInputAction* UNyotaInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const auto& Action : PlayerInput) {

		if (Action.Input && (Action.TagName == InputTag)) {

			return Action.Input;

		}

	}

	UE_LOG(LogTemp, Warning, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}
