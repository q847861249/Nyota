// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CustomAbilitySystemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYOTA_API UCustomAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    virtual void OnGiveAbility(FGameplayAbilitySpec &AbilitySpec) override;

    virtual void OnRep_ActivateAbilities() override;

    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    void SetAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level);

    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    void AddToAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level = 1);

    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    UGameplayAbility *GetActivatableAbilitySpecByTag(const FGameplayTag &AbilityTag);

private:
    void HandleAutoActivateAbility(const FGameplayAbilitySpec &AbilitySpec);
};
