// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/CustomAbilitySystemComponent.h"

#include "Abilities/GameplayAbility.h"
#include "GameplayTags/GameTags.h"

void UCustomAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec &AbilitySpec)
{
    Super::OnGiveAbility(AbilitySpec);

    HandleAutoActivateAbility(AbilitySpec);
}

void UCustomAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    // 防止遍历期间能力列表被修改，避免迭代器失效
    FScopedAbilityListLock ScopedAbilityListLock(*this);
    for (const FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        HandleAutoActivateAbility(AbilitySpec);
    }
}

void UCustomAbilitySystemComponent::HandleAutoActivateAbility(const FGameplayAbilitySpec &AbilitySpec)
{
    if (!IsValid(AbilitySpec.Ability))
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Tags: %s"), *AbilitySpec.Ability->AbilityTags.ToString());
    for (const FGameplayTag &Tag : AbilitySpec.Ability->AbilityTags)
    {
        if (!Tag.MatchesTagExact(GameTags::Abilities::ActivateOnGive))
        {
            continue;
        }

        TryActivateAbility(AbilitySpec.Handle);
    }
}