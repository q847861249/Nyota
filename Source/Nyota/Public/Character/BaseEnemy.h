// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseEnemy.generated.h"

class UAttributeSet;

UCLASS()
class NYOTA_API ABaseEnemy : public ABaseCharacter
{
    GENERATED_BODY()

public:
    ABaseEnemy();

    virtual void BeginPlay() override;

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    virtual UAttributeSet *GetAttributeSet() const override;
protected:
    void InitStartingAbilities();
private:
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<class UGameplayAbility>> StartingAbilities;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
};
