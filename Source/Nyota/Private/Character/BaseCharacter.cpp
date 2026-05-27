// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameplayTags/GameTags.h"
ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent *ABaseCharacter::GetAbilitySystemComponent() const
{
    return nullptr;
}

UAttributeSet *ABaseCharacter::GetAttributeSet() const
{
    return nullptr;
}

bool ABaseCharacter::IsAlive() const
{
    return bAlive;
}

void ABaseCharacter::SetAlive(bool bAliveStatus)
{
    bAlive = bAliveStatus;
}

void ABaseCharacter::GiveDefaultAbility()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

    if (!IsValid(ASC))
    {
        return;
    }

    // if (!IsValid(GetAbilitySystemComponent()))
    // {
    //     return;
    // }

    for (const auto &Ability : GAClass)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
        ASC->GiveAbility(AbilitySpec);

        //判断技能是否有init标签，有的直接激活
        const UGameplayAbility* AbilityObj = Ability->GetDefaultObject<UGameplayAbility>();

        if (AbilityObj && AbilityObj->AbilityTags.HasTagExact(Nyota::Ability_init))
        {
            ASC->TryActivateAbility(AbilitySpec.Handle);
        }
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

void ABaseCharacter::OnHealthChange(const FOnAttributeChangeData &AttributeChangeData)
{
    if (AttributeChangeData.NewValue <= 0.f)
    {
        OnDeath();
    }
}

void ABaseCharacter::OnDeath()
{
    bAlive = false;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s has die!"), *GetName()));
    }
}

void ABaseCharacter::OnRespawn()
{
    bAlive = true;
}

EPlayerType ABaseCharacter::GetPlayerType() const
{
    return PlayerType;
}

FVector ABaseCharacter::GetForwardDirection() const
{
    // if (GetPlayerType() == EPlayerType::Crab)
    // {
    //     return GetActorRightVector();
    // }

    return GetActorForwardVector();
}
