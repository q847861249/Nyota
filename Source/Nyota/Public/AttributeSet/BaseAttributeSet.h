// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                   \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                         \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                       \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttributesInitialized);

/**
 *
 */
UCLASS()
class NYOTA_API UBaseAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const override;

    /**
     * @brief GE 执行前的属性修改拦截：实现"护盾吸收伤害 + 减伤"的结算规则。
     * @details 对 Health 的负向修改（伤害）按顺序处理：
     *          1. 乘以 (1 - DamageReduction)（泥土护盾的防御力提升，GE 限时增益自动到期还原）；
     *          2. 优先从 Shield 扣除，剩余部分才落到 Health。
     *          护盾值为 0 或增益不存在时行为与原生一致，不影响其他技能的伤害 GE。
     * @return false 表示拦截本次属性修改（当前不使用，始终走 Super）。
     */
    virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;

    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData &OldValue);

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData &OldValue);

    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData &OldValue);

    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData &OldValue);

    UFUNCTION()
    void OnRep_Coin(const FGameplayAttributeData &OldValue);

    UFUNCTION()
    void OnRep_MaxCoin(const FGameplayAttributeData &OldValue);

    UFUNCTION()
    void OnRep_Shield(const FGameplayAttributeData &OldValue);

    UFUNCTION()
    void OnRep_DamageReduction(const FGameplayAttributeData &OldValue);
    
    UFUNCTION()
    void OnRep_AttributesInitialized();

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(ThisClass, Health);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana)
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(ThisClass, Mana);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(ThisClass, MaxMana);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Coin)
    FGameplayAttributeData Coin;
    ATTRIBUTE_ACCESSORS(ThisClass, Coin);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCoin)
    FGameplayAttributeData MaxCoin;
    ATTRIBUTE_ACCESSORS(ThisClass, MaxCoin);

    /**
     * @brief 当前护盾值：受到伤害时优先消耗，护盾归零前 Health 不掉血。
     * @details 不设 MaxShield——护盾量由技能（泥土护盾）一次性写入基础值，吸收殆尽自然归零。
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield)
    FGameplayAttributeData Shield;
    ATTRIBUTE_ACCESSORS(ThisClass, Shield);

    /**
     * @brief 伤害减免系数，取值 0~1：0 = 无减免，0.3 = 受到的伤害打七折。
     * @details 由增益 GE（如泥土护盾的防御力提升）以 Additive 修饰符临时增加，GE 到期聚合器自动还原，
     *          结算发生在 PreGameplayEffectExecute 里。
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageReduction)
    FGameplayAttributeData DamageReduction;
    ATTRIBUTE_ACCESSORS(ThisClass, DamageReduction);
    
    UPROPERTY(ReplicatedUsing = OnRep_AttributesInitialized)
    bool bAttributesInitialized = false;
    
    UPROPERTY(BlueprintAssignable)
    FAttributesInitialized OnAttributesInitialized;
};
