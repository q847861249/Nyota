// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_OpenMap.generated.h"

/**
 * 
 */
class UOpenMap;
UCLASS()
class NYOTA_API UGA_OpenMap : public UNyotaGameplayAbility
{
	GENERATED_BODY()
public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;
	
	UFUNCTION()
	void OnInputReleaseAgain(float TimeWaited);
private:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UOpenMap> MapClass;

    UPROPERTY()
    TObjectPtr<UOpenMap> MapWidget;

    void OpenMap();
    void CloseMap();
};
