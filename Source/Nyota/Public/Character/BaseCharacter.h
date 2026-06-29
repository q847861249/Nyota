// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "Enums/PlayerType.h"
#include "ModularCharacter.h"

#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent *, ASC, UAttributeSet *, AS);

class UGameplayEffect;

UCLASS(Config = Game)
class NYOTA_API ABaseCharacter : public AModularCharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    virtual UAttributeSet *GetAttributeSet() const;

    bool IsAlive() const;

    void SetAlive(bool bAliveStatus);

    virtual void OnRespawn();

    virtual void PawnClientRestart() override;

    virtual UInputComponent *CreatePlayerInputComponent() override;

    EPlayerType GetPlayerType() const;

    FVector GetForwardDirection() const;

    UPROPERTY(BlueprintAssignable)
    FASCInitialized OnASCInitialized;

protected:
    void GiveDefaultAbility();

    void InitializeAttributes() const;

    void OnHealthChange(const FOnAttributeChangeData &AttributeChangeData);

    virtual void OnDeath();

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Crash | Abilities")
    TArray<TSubclassOf<UGameplayAbility>> GAClass;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Crash | Effects")
    TSubclassOf<UGameplayEffect> InitializeAttributesEffect;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bAlive = true;

    UPROPERTY(EditDefaultsOnly, Category = "Enums")
    EPlayerType PlayerType;
};