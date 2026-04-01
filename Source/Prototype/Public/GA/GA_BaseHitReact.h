// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BaseHitReact.generated.h"

class ABaseCharacter;

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
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/**
	 * @brief Niagara特效附加函数，用于在指定位置和旋转角度附加特效
	 *
	 */
	UFUNCTION()
	void NiaAttach(UNiagaraSystem *NiagaraSystem, FVector HitLocation, FRotator HitRotation);

	/**
	 * @brief 攻击处理函数，处理攻击相关的逻辑，包括攻击时间、动画蒙太奇和特效
	 *
	 */
	UFUNCTION()
	void AttackAbout(ABaseCharacter *BaseCharacter, float AttackTime, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect, TSubclassOf<UGameplayEffect> HitEffectClass);

	/**
	 * @brief 攻击处理函数，处理攻击相关的逻辑，包括攻击时间、动画蒙太奇和特效
	 *
	 */
	UFUNCTION()
	void Attack(ABaseCharacter *BaseCharacter, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect, TSubclassOf<UGameplayEffect> HitEffectClass);

	UPROPERTY(BlueprintReadWrite)
	ABaseCharacter *AvatarCharacter;

	UPROPERTY()
	TArray<AActor *> HitActors;
};
