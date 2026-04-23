// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/AttributeChangeTask.h"

#include "AbilitySystemComponent.h"

UAttributeChangeTask *UAttributeChangeTask::ListenForAttributeChange(
    UAbilitySystemComponent *AbilitySystemComponent, FGameplayAttribute Attribute
)
{
    // 创建异步任务对象
    UAttributeChangeTask *AttributeChangeTask = NewObject<UAttributeChangeTask>();

    // 缓存 ASC 弱引用和目标属性，供后续回调使用
    AttributeChangeTask->ASC = AbilitySystemComponent;
    AttributeChangeTask->GameplayAttribute = Attribute;

    // 提前校验 ASC 有效性，无效则清理任务并返回空
    if (!IsValid(AbilitySystemComponent))
    {
        // 移除 Root 引用，允许 GC 回收该对象
        AttributeChangeTask->RemoveFromRoot();

        return nullptr;
    }

    // 向 ASC 注册属性变化委托，属性值变化时触发 AttributeChanged 回调
    AttributeChangeTask->ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(
        AttributeChangeTask, &UAttributeChangeTask::AttributeChanged
    );

    return AttributeChangeTask;
}

void UAttributeChangeTask::EndTask()
{
    // ASC 仍有效时，移除本对象绑定的所有属性变化委托，防止回调野指针
    if (ASC.IsValid())
    {
        ASC->GetGameplayAttributeValueChangeDelegate(GameplayAttribute).RemoveAll(this);
    }

    // 通知基类该异步任务已完成，释放相关资源
    SetReadyToDestroy();

    // 标记为垃圾，等待 GC 在下一帧回收
    MarkAsGarbage();
}

void UAttributeChangeTask::AttributeChanged(const FOnAttributeChangeData &Data)
{
    // 将属性变化事件广播出去（属性、新值、旧值）
    OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}