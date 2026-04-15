// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"

#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);

class UGameplayEffect;

UCLASS()
class PROTOTYPE_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter();

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    /**
     * @brief 设置死亡
     *
     */
    UFUNCTION(BlueprintCallable)
    void SetDead();

protected:
    void GiveDefaultAbility();

    void InitializeAttributes() const;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Crash | Abilities")
    TArray<TSubclassOf<UGameplayAbility>> GAClass;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Crash | Effects")
    TSubclassOf<UGameplayEffect> InitializeAttributesEffect;
};
