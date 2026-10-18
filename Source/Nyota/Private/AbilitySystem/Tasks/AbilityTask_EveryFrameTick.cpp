// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Tasks/AbilityTask_EveryFrameTick.h"

UAbilityTask_EveryFrameTick::UAbilityTask_EveryFrameTick(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    // 让 ASC（UGameplayTasksComponent 基类）把本任务加入 TickingTasks 并每帧回调 TickTask；
    // 任务激活期间组件自动开启 Tick，最后一个 Tick 任务结束时自动关闭
    bTickingTask = true;
}

UAbilityTask_EveryFrameTick *UAbilityTask_EveryFrameTick::EveryFrameTick(UGameplayAbility *OwningAbility)
{
    UAbilityTask_EveryFrameTick *Task = NewAbilityTask<UAbilityTask_EveryFrameTick>(OwningAbility);
    // 必须用 ReadyForActivation 而不是 Activate()：基类 UGameplayTask::Activate() 是空实现，
    // 不会把任务注册进 ASC 的 TickingTasks；只有 ReadyForActivation → PerformActivation 才会
    // 调用 TasksComponent->OnGameplayTaskActivated，把 bTickingTask 任务加入 TickingTasks 并开启组件 Tick
    Task->ReadyForActivation();

    return Task;
}

void UAbilityTask_EveryFrameTick::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    OnEveryFrameTick.Broadcast(DeltaTime);
}
