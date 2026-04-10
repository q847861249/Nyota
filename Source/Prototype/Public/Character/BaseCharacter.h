// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"

#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);

UCLASS()
class PROTOTYPE_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter();

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    /**
     * @brief 设置被击中
     *
     */
    UFUNCTION(BlueprintCallable)
    void SetHit();

    /**
     * @brief 应用生命值变化
     *
     */
    UFUNCTION(BlueprintCallable)
    void ApplyHealthChange(float DeltaValue);

    /**
     * @brief 设置死亡
     *
     */
    UFUNCTION(BlueprintCallable)
    void SetDead();
    
    UPROPERTY()
    bool bIsHit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (ClampMin = "0.0"))
    float MaxHealth = 10.0f;

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void GiveDefaultAbility();

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> GAClass;

    FTimerHandle HitTimerHandle;
};
