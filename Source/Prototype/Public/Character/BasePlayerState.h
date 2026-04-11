// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "BasePlayerState.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()
    
public:
    ABasePlayerState();
    
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Crash|Abilities")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
