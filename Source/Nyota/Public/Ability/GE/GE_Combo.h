// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Combo.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UGE_Combo : public UGameplayEffect
{
	GENERATED_BODY()
	
public:

	virtual void PostInitProperties() override;

	//void InternalOnActiveGameplayEffectAdded(FActiveGameplayEffect& Effect, bool bInvokeGameplayCueEvents);

};
