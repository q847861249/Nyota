// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_BaseSkill.h"
#include "GA_VortexGrip.generated.h"

class ABaseCharacter;
/**
 *
 */
UCLASS()
class PROTOTYPE_API UGA_VortexGrip : public UGA_BaseSkill
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    UFUNCTION()
    void OnEventReceived(FGameplayEventData EventData);

    UFUNCTION()
    void ApplyDamage(FGameplayEventData EventData);

    UFUNCTION()
    void OnAbilityEnd();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TObjectPtr<UAnimMontage> VortexGripMontage;

    UPROPERTY(BlueprintReadOnly, Category = "Nyota")
    TWeakObjectPtr<ABaseCharacter> GrabbedCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    TSubclassOf<UGameplayEffect> VortexGripEffect;
};
