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
class UNyotaStatusComponent;

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
    void InitializeAttributes() const;

    void OnHealthChange(const FOnAttributeChangeData &AttributeChangeData);

    virtual void OnDeath();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Crash | Effects")
    TSubclassOf<UGameplayEffect> InitializeAttributesEffect;

    /** 受控状态组件：监听屏蔽维度 Tag 并执行冻结反应，角色类本身不关心具体有哪些状态。 */
    UPROPERTY(VisibleAnywhere, Category = "Nyota | Status")
    TObjectPtr<UNyotaStatusComponent> StatusComponent;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bAlive = true;

    UPROPERTY(EditDefaultsOnly, Category = "Enums")
    EPlayerType PlayerType;
};