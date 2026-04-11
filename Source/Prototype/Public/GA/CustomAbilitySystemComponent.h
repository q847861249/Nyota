// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CustomAbilitySystemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTOTYPE_API UCustomAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    virtual void OnGiveAbility(FGameplayAbilitySpec &AbilitySpec) override;

    virtual void OnRep_ActivateAbilities() override;

private:
    void HandleAutoActivateAbility(const FGameplayAbilitySpec &AbilitySpec);
};
