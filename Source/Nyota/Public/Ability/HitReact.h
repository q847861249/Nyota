// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "HitReact.generated.h"

class UAbilitySystemComponent;
class ABaseCharacter;
UCLASS()
class NYOTA_API UHitReact : public UBaseAbility
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	float DeathDropRate = 4.0f;
	TObjectPtr<UAbilitySystemComponent> ASC;
	TObjectPtr<ABaseCharacter> Instigator;
	
	void OnHealthValueChange(const struct FOnAttributeChangeData& Data);
public:
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(TSubclassOf<UGameplayEffect> GEClass, float Damage, AActor* OutInstigator);
	UFUNCTION(BlueprintCallable)
	void ListenHealth();
};
