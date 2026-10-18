// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_EveryFrameTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskEveryFrameTickDelegate, float, DeltaTime);

/**
 * @brief 通用"每帧回调"技能任务：激活期间每帧广播一次 OnEveryFrameTick。
 * @details UE5.4 的 UGameplayAbility 没有 Tick 入口，引擎标准做法是用 bTickingTask 的
 *          UGameplayTask 借道 UGameplayTasksComponent（ASC 基类）获得每帧回调：
 *          任务激活时 ASC 自动开启组件 Tick，结束时自动关闭，无需手动管理计时器，
 *          且天然帧率无关（每帧恰好回调一次，卡帧不补触发）。
 *          任何需要在技能生命周期内逐帧执行的逻辑（位移驱动、持续检测等）都可复用本任务。
 */
UCLASS()
class NYOTA_API UAbilityTask_EveryFrameTick : public UAbilityTask
{
    GENERATED_BODY()

public:
    /**
     * @brief 创建并激活一个每帧回调任务。
     * @param OwningAbility 拥有此任务的技能。
     * @return 已激活的任务实例；调用方绑定 OnEveryFrameTick 后每帧收到回调。
     */
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
        meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
    static UAbilityTask_EveryFrameTick *EveryFrameTick(UGameplayAbility *OwningAbility);

    /** 每帧触发一次，DeltaTime 为本帧时长，单位：秒。 */
    UPROPERTY(BlueprintAssignable)
    FAbilityTaskEveryFrameTickDelegate OnEveryFrameTick;

    virtual void TickTask(float DeltaTime) override;

protected:
    UAbilityTask_EveryFrameTick(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());
};
