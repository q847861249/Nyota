// Copyright Nyota Project. All Rights Reserved.

#include "AbilitySystem/NyotaAbilitySet.h"

#include "AbilitySystem/NyotaAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"

// ============================================================================
// FNyotaAbilitySet_GrantedHandles — 授予句柄集合
// ============================================================================
// 记录一次 GiveToAbilitySystem 调用所授予的全部句柄（技能、效果、属性集），
// 方便后续通过 TakeFromAbilitySystem 一次性全部移除。

void FNyotaAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle &Handle)
{
    if (Handle.IsValid())
    {
        AbilitySpecHandles.Add(Handle);
    }
}

void FNyotaAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle &Handle)
{
    if (Handle.IsValid())
    {
        GameplayEffectHandles.Add(Handle);
    }
}

void FNyotaAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet *Set)
{
    GrantedAttributeSets.Add(Set);
}

void FNyotaAbilitySet_GrantedHandles::TakeFromAbilitySystem(UNyotaAbilitySystemComponent *NyotaASC)
{
    check(NyotaASC);

    // 授予和移除技能集必须在权威端（服务器）执行，客户端仅同步结果。
    if (!NyotaASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    // 逐项清除之前授予的技能、效果和属性集

    for (const FGameplayAbilitySpecHandle &Handle : AbilitySpecHandles)
    {
        if (Handle.IsValid())
        {
            NyotaASC->ClearAbility(Handle);
        }
    }

    for (const FActiveGameplayEffectHandle &Handle : GameplayEffectHandles)
    {
        if (Handle.IsValid())
        {
            NyotaASC->RemoveActiveGameplayEffect(Handle);
        }
    }

    for (UAttributeSet *Set : GrantedAttributeSets)
    {
        NyotaASC->RemoveSpawnedAttribute(Set);
    }

    // 清空句柄记录，防止重复移除
    AbilitySpecHandles.Reset();
    GameplayEffectHandles.Reset();
    GrantedAttributeSets.Reset();
}

// ============================================================================
// UNyotaAbilitySet — 技能集数据资产
// ============================================================================

UNyotaAbilitySet::UNyotaAbilitySet(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNyotaAbilitySet::GiveToAbilitySystem(
    UNyotaAbilitySystemComponent *NyotaASC, FNyotaAbilitySet_GrantedHandles *OutGrantedHandles, UObject *SourceObject
) const
{
    check(NyotaASC);

    // 授予和移除技能集必须在权威端（服务器）执行
    if (!NyotaASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    // ---- 授予属性集 ----
    // 为配置中指定的每个属性集类型创建实例，挂载到 ASC 上。
    // 属性集是角色属性的载体（如生命值、体力值等），以 SubObject 形式存在。
    for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
    {
        const FNyotaAbilitySet_AttributeSet &SetToGrant = GrantedAttributes[SetIndex];

        if (!IsValid(SetToGrant.AttributeSet))
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"),
                SetIndex,
                *GetNameSafe(this)
            );
            continue;
        }

        // 使用 ASC 的 Owner 作为 Outer，确保属性集的生命周期与所属 Actor 绑定
        UAttributeSet *NewSet = NewObject<UAttributeSet>(NyotaASC->GetOwner(), SetToGrant.AttributeSet);
        NyotaASC->AddAttributeSetSubobject(NewSet);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAttributeSet(NewSet);
        }
    }

    // ---- 授予技能 ----
    // 从配置的技能类创建 AbilitySpec 并注册到 ASC。
    // AbilitySpec 携带技能等级、源对象、以及用于输入绑定的 InputTag。
    for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
    {
        const FNyotaAbilitySet_GameplayAbility &AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

        if (!IsValid(AbilityToGrant.Ability))
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."),
                AbilityIndex,
                *GetNameSafe(this)
            );
            continue;
        }

        // 获取技能的 CDO（Class Default Object），用于构造 AbilitySpec
        UNyotaGameplayAbility *AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UNyotaGameplayAbility>();

        FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
        AbilitySpec.SourceObject = SourceObject;
        // 将配置的 InputTag 写入 DynamicAbilityTags，供输入系统匹配技能
        AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

        const FGameplayAbilitySpecHandle AbilitySpecHandle = NyotaASC->GiveAbility(AbilitySpec);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
        }
    }

    // ---- 授予持续效果 ----
    // 将配置的 GameplayEffect 直接应用到自身（ApplyGameplayEffectToSelf）。
    // 通常用于授予被动效果，如基础属性初始化、永久 Buff 等。
    for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
    {
        const FNyotaAbilitySet_GameplayEffect &EffectToGrant = GrantedGameplayEffects[EffectIndex];

        if (!IsValid(EffectToGrant.GameplayEffect))
        {
            UE_LOG(
                LogTemp,
                Error,
                TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"),
                EffectIndex,
                *GetNameSafe(this)
            );
            continue;
        }

        const UGameplayEffect *GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
        const FActiveGameplayEffectHandle GameplayEffectHandle = NyotaASC->ApplyGameplayEffectToSelf(
            GameplayEffect, EffectToGrant.EffectLevel, NyotaASC->MakeEffectContext()
        );

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
        }
    }
}