// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BaseHitReact.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackFinished); // 攻击完成回调函数

class ABaseCharacter;
class UNiagaraSystem;

/**
 * 所有角色技能的基础 GameplayAbility：
 * - 在激活时缓存施法者（TestChar）
 * - 提供统一的命中特效播放（Niagara）
 * - 提供通用的近战范围攻击逻辑（Attack / AttackAbout）
 */
UCLASS()
class PROTOTYPE_API UGA_BaseHitReact : public UGameplayAbility
{
    GENERATED_BODY()

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
    ) override;

    /**
     * @brief Niagara特效附加函数，用于在指定位置和旋转角度附加特效
     *
     */
    UFUNCTION()
    void NiaAttach(UNiagaraSystem *NiagaraSystem, FVector SpawnLocation, FRotator SpawnRotation);

    /**
     * @brief 攻击处理函数，处理攻击相关的逻辑，包括攻击时间、动画蒙太奇和特效
     *
     */
    void DelayAttack(
        ABaseCharacter *AttackCharacter, float AttackTime, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect,
        TSubclassOf<UGameplayEffect> HitEffectClass
    );

    /**
     * @brief 攻击处理函数，处理攻击相关的逻辑，包括攻击时间、动画蒙太奇和特效
     *
     */
    void Attack(
        ABaseCharacter *AttackCharacter, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect,
        TSubclassOf<UGameplayEffect> HitEffectClass
    );
    
public:
    FOnAttackFinished OnAttackFinished;

protected:
    // 施法者
    UPROPERTY(BlueprintReadWrite)
    ABaseCharacter *AvatarCharacter;

    // 命中目标列表
    UPROPERTY()
    TArray<AActor *> HitActors;
};
