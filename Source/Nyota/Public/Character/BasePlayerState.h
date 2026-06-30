// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"

#include "BasePlayerState.generated.h"

class UNyotaExperienceDefinition;
class UNyotaPawnData;
class UNyotaAbilitySystemComponent;
class UAttributeSet;

/**
 *
 */
UCLASS(Config = Game)
class NYOTA_API ABasePlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ABasePlayerState(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    //~AActor interface
    virtual void PreInitializeComponents() override;

    virtual void PostInitializeComponents() override;
    //~End of AActor interface

    UNyotaAbilitySystemComponent *GetNyotaAbilitySystemComponent() const;

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    UAttributeSet *GetAttributeSet() const;

    template <class T> const T *GetPawnData() const
    {
        return Cast<T>(PawnData);
    }

    void SetPawnData(const UNyotaPawnData *InPawnData);

    static const FName NAME_NyotaAbilityReady;

protected:
    UFUNCTION()
    void OnRep_PawnData();

    UPROPERTY(ReplicatedUsing = OnRep_PawnData)
    TObjectPtr<const UNyotaPawnData> PawnData;

private:
    void OnExperienceLoaded(const UNyotaExperienceDefinition *CurrentExperience);

    UPROPERTY(VisibleAnywhere, Category = "Nyota | Abilities")
    TObjectPtr<UNyotaAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
};
