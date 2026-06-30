// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitInputActionReleased.generated.h"

class UInputAction;
class UEnhancedInputComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitInputActionReleasedDelegate, float, TimeHeld);

/**
 * 监听增强输入 Action 的 Completed 事件（按键松开）。
 * 用于替代 UAbilityTask_WaitInputRelease —— 后者在标签激活（TryActivateAbilitiesByTag）的项目中无法工作。
 */
UCLASS()
class NYOTA_API UAbilityTask_WaitInputActionReleased : public UAbilityTask
{
	GENERATED_BODY()

public:
	/** 按键松开时触发，TimeHeld 为从任务激活到松开的持续时间（秒） */
	UPROPERTY(BlueprintAssignable)
	FWaitInputActionReleasedDelegate OnRelease;

	/**
	 * 创建一个等待 InputAction 松开的任务。
	 *
	 * @param OwningAbility 拥有此任务的技能
	 * @param InputAction   要监听的增强输入 Action
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
	    meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitInputActionReleased *WaitInputActionReleased(
	    UGameplayAbility *OwningAbility, UInputAction *InputAction);

	virtual void Activate() override;

protected:
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void OnInputActionCompleted();

	UPROPERTY()
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> CachedInputComponent;

	uint32 BindingHandle = 0;

	float StartTime = 0.0f;
};