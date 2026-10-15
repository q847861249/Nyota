// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"

#include "GA_PutDown.generated.h"

/** @brief 执行抓取后的放下或抛出派生动作，并通知抓取技能清理状态。 */
UCLASS()
class NYOTA_API UGA_PutDown : public UNyotaGameplayAbility
{
    GENERATED_BODY()

#if WITH_DEV_AUTOMATION_TESTS
    friend class FPutDownAbilityContractTest;
#endif

public:
    /** @brief 在构造阶段设置放下技能标识，并要求抓取技能已开启派生窗口。 */
    UGA_PutDown(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota")
    float ThrownForce;
};
