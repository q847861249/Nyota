// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "BaseWidgetComponent.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UBaseAttributeSet;
class ABaseCharacter;
class UCustomAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROTOTYPE_API UBaseWidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    void InitAbilitySystemData();

    bool IsASCInitialized() const;
    
    void InitializeAttributeDelegate();

    UFUNCTION()
    void OnASCInitialized(UAbilitySystemComponent *ASC, UAttributeSet *AS);
    
    UFUNCTION()
    void BindToAttributeChange();

    TWeakObjectPtr<ABaseCharacter> BaseCharacter;
    TWeakObjectPtr<UBaseAttributeSet> BaseAttributeSet;
    TWeakObjectPtr<UCustomAbilitySystemComponent> BaseAbilitySystemComponent;
};