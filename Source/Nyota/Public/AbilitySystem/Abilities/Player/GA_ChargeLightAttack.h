// Fill out your copyright notice in Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_ChargeLightAttack.generated.h"

class ABaseCharacter;
class UGA_Charge;

/**
 * @brief 冲锋派生普攻（技能一的推土机衍生操作）。
 * @details 仅在冲锋进入推土机状态（Ability_State_Charging_Bulldozer）时可激活：
 *          把被推的敌人顶飞并结束推动，同时冲锋技能随之结束。
 *          本类只负责触发顶飞逻辑，动画与打击特效由蓝图事件表现。
 */
UCLASS()
class NYOTA_API UGA_ChargeLightAttack : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_ChargeLightAttack(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
    ) override;

protected:
    /** 蓝图播放顶飞表现（挥击/撞击特效）。 */
    UFUNCTION(BlueprintImplementableEvent, Category = "Nyota | Charge")
    void OnLaunchEnemy_Visual(ABaseCharacter *LaunchedEnemy);

private:
    /**
     * @brief 找到当前正在运行的冲锋技能实例。
     * @return 冲锋技能实例；未处于冲锋中返回 nullptr。
     */
    UGA_Charge *FindActiveChargeAbility() const;
};
