// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent *ABaseCharacter::GetAbilitySystemComponent() const
{
    return nullptr;
}

UAttributeSet * ABaseCharacter::GetAttributeSet() const
{
    return nullptr;
}

void ABaseCharacter::SetDead()
{
    GetMesh()->SetAllBodiesSimulatePhysics(true);       // 设置网格体模拟物理
    GetCharacterMovement()->SetMovementMode(MOVE_None); // 停止移动
}

void ABaseCharacter::GiveDefaultAbility()
{
    if (!IsValid(GetAbilitySystemComponent()))
    {
        return;
    }

    for (const auto &Ability : GAClass)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
        GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
    }
}

void ABaseCharacter::InitializeAttributes() const
{
    checkf(InitializeAttributesEffect, TEXT("InitializeAttributesEffect not set."));

    FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle =
        GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributesEffect, 1.f, ContextHandle);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
