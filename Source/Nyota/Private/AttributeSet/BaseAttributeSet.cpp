// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSet/BaseAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Coin, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxCoin, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Shield, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, DamageReduction, COND_None, REPNOTIFY_Always);
    
    // DOREPLIFETIME(ThisClass, bAttributesInitialized);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, bAttributesInitialized, COND_None, REPNOTIFY_Always);
}

bool UBaseAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data)
{
    // 只拦截对 Health 的伤害（负向修改）；治疗/其他属性原样放行
    if (Data.EvaluatedData.Attribute == GetHealthAttribute() && Data.EvaluatedData.Magnitude < 0.f)
    {
        // 1. 防御力提升：伤害乘以 (1 - DamageReduction)，由增益 GE 临时提供，到期自动还原
        float Damage = -Data.EvaluatedData.Magnitude * (1.f - FMath::Clamp(GetDamageReduction(), 0.f, 1.f));

        // 2. 护盾优先吸收：能吸多少吸多少，剩余伤害才写入 Health
        if (GetShield() > 0.f && Damage > 0.f)
        {
            const float Absorbed = FMath::Min(GetShield(), Damage);

            // 直接改基础值（绕过聚合器），避免与增益 GE 的修饰符互相污染
            SetShield(FMath::Max(0.f, GetShield() - Absorbed));
            Damage -= Absorbed;
        }

        // 把结算后的剩余伤害写回，Health 只会收到护盾吸收后的部分
        Data.EvaluatedData.Magnitude = -Damage;
    }

    return Super::PreGameplayEffectExecute(Data);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
    Super::PostGameplayEffectExecute(Data);
    
    if (!bAttributesInitialized)
    {
        bAttributesInitialized = true;
        
        OnAttributesInitialized.Broadcast();
    }
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);
}

void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldValue);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldValue);
}

void UBaseAttributeSet::OnRep_Coin(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Coin, OldValue);
}

void UBaseAttributeSet::OnRep_MaxCoin(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxCoin, OldValue);
}

void UBaseAttributeSet::OnRep_Shield(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Shield, OldValue);
}

void UBaseAttributeSet::OnRep_DamageReduction(const FGameplayAttributeData &OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, DamageReduction, OldValue);
}

void UBaseAttributeSet::OnRep_AttributesInitialized()
{
    if (bAttributesInitialized)
    {
        OnAttributesInitialized.Broadcast();
    }
}