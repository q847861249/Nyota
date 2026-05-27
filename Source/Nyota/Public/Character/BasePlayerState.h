// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "BasePlayerState.generated.h"

class UAttributeSet;

/**
 *
 */
UCLASS()
class NYOTA_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ABasePlayerState();

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    UAttributeSet *GetAttributeSet() const;

private:
    UPROPERTY(VisibleAnywhere, Category = "Nyota | Abilities")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
};
