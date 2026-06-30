// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BaseSkill.generated.h"

UENUM(BlueprintType)
enum class ENyotaAbilityActivationPolicy : uint8
{
    // 当输入被触发时，尝试激活该能力。
    OnInputTriggered,

    // 在输入处于激活状态时，持续尝试激活该能力。
    WhileInputActive,

    // Try to activate the ability when an avatar is assigned.
    OnSpawn
};

/**
 *
 */
UCLASS()
class NYOTA_API UGA_BaseSkill : public UGameplayAbility
{
    GENERATED_BODY()

    friend class UNyotaAbilitySystemComponent;

public:
    UGA_BaseSkill(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    ENyotaAbilityActivationPolicy GetActivatePolicy() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Debug")
    bool bDrawDebugs = false;

protected:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyota | OverlapTest")
    FVector GetAbilityDetectionDirection() const;

    virtual FVector GetAbilityDetectionDirection_Implementation() const;

    // 定义如何激活这种能力。
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Ability Activation")
    ENyotaAbilityActivationPolicy ActivationPolicy;
};
