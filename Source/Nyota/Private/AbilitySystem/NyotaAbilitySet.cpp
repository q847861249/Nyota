// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/NyotaAbilitySet.h"

#include "AbilitySystem/NyotaAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"

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

    if (!NyotaASC->IsOwnerActorAuthoritative())
    {
        // Must be authoritative to give or take ability sets.
        return;
    }

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

    AbilitySpecHandles.Reset();
    GameplayEffectHandles.Reset();
    GrantedAttributeSets.Reset();
}

UNyotaAbilitySet::UNyotaAbilitySet(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNyotaAbilitySet::GiveToAbilitySystem(
    UNyotaAbilitySystemComponent *NyotaASC, FNyotaAbilitySet_GrantedHandles *OutGrantedHandles, UObject *SourceObject
) const
{

    check(NyotaASC);

    if (!NyotaASC->IsOwnerActorAuthoritative())
    {
        // Must be authoritative to give or take ability sets.
        return;
    }

    // Grant the attribute sets.
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

        UAttributeSet *NewSet = NewObject<UAttributeSet>(NyotaASC->GetOwner(), SetToGrant.AttributeSet);
        NyotaASC->AddAttributeSetSubobject(NewSet);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAttributeSet(NewSet);
        }
    }

    // Grant the gameplay abilities.
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

        UNyotaGameplayAbility *AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UNyotaGameplayAbility>();

        FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
        AbilitySpec.SourceObject = SourceObject;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

        const FGameplayAbilitySpecHandle AbilitySpecHandle = NyotaASC->GiveAbility(AbilitySpec);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
        }
    }

    // Grant the gameplay effects.
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