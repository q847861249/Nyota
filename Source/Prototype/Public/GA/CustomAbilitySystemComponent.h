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
    UCustomAbilitySystemComponent();

    virtual void TickComponent(
        float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction
    ) override;

protected:
    virtual void BeginPlay() override;

private:
};
