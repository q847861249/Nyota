// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularGameState.h"

#include "NyotaGameState.generated.h"

class UCustomAbilitySystemComponent;
class UNyotaExperienceManagerComponent;
/**
 *
 */
UCLASS(Config = Game)
class NYOTA_API ANyotaGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ANyotaGameState(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    //~IAbilitySystemInterface
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;
    //~End of IAbilitySystemInterface

private:
    UPROPERTY(VisibleAnywhere, Category = "Nyota | GameState")
    TObjectPtr<UCustomAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, Category = "Nyota | GameState")
    TObjectPtr<UNyotaExperienceManagerComponent> ExperienceManagerComponent;
};
