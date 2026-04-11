// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseEnemy.generated.h"

UCLASS()
class PROTOTYPE_API ABaseEnemy : public ABaseCharacter
{
    GENERATED_BODY()

public:
    ABaseEnemy();
    
    virtual void BeginPlay() override;
    
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;
    
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
