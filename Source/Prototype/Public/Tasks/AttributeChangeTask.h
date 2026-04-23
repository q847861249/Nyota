// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AttributeChangeTask.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue
);

/**
 *
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class PROTOTYPE_API UAttributeChangeTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnAttributeChanged OnAttributeChanged;

    /**
     * 创建并注册一个属性变化监听任务
     *
     * @param AbilitySystemComponent  要监听的目标 ASC
     * @param Attribute               要监听的具体 Gameplay 属性
     * @return 创建成功返回任务实例，ASC 无效则返回 nullptr
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UAttributeChangeTask *ListenForAttributeChange(
        UAbilitySystemComponent *AbilitySystemComponent, FGameplayAttribute Attribute
    );

    /**
     * 手动结束任务，注销委托并标记对象等待销毁
     * 建议在不再需要监听时主动调用，避免悬空委托
     */
    UFUNCTION(BlueprintCallable)
    void EndTask();

    /**
     * 属性值变化时的回调函数，由 ASC 委托触发
     * 将变化数据广播给所有蓝图/C++ 监听者
     *
     * @param Data  包含变化属性、新值、旧值的结构体
     */
    void AttributeChanged(const FOnAttributeChangeData &Data);

    TWeakObjectPtr<UAbilitySystemComponent> ASC;

    FGameplayAttribute GameplayAttribute;
};
