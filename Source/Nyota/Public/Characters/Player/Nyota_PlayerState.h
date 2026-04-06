// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Nyota_PlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class NYOTA_API ANyota_PlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ANyota_PlayerState();
private:
	UPROPERTY(VisibleAnywhere, Category =  "AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> BaseAttributeSet;

protected:

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetBaseAttributeSet() const;


};
