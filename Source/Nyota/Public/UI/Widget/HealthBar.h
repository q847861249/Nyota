// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;
class UAbilitySystemComponent;

UCLASS()
class NYOTA_API UHealthBar : public UCommonUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

	void SetHealthBar();
public:
	/** Initial the health bar widget by passing AbilitySystemComponent parameter
	 * @param OutASC the pointer of AbilitySystemComponent 
	 */
	UFUNCTION(BlueprintCallable)
	void InitialHealthBar(UAbilitySystemComponent* OutASC);
};
