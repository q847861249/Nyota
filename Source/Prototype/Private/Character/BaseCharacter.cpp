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

void ABaseCharacter::SetHit()
{
    bIsHit = true;

    if (UWorld *World = GetWorld())
    {
        World->GetTimerManager().SetTimer(HitTimerHandle, [this]() { bIsHit = false; }, 0.5f, false);
    }
}

void ABaseCharacter::ApplyHealthChange(float DeltaValue)
{
    CurrentHealth += DeltaValue;

    CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, FMath::Max(0.f, MaxHealth));

    OnHealthChanged.Broadcast(CurrentHealth, FMath::Max(0.f, MaxHealth));
}

void ABaseCharacter::SetDead()
{
    GetMesh()->SetAllBodiesSimulatePhysics(true);                      // 设置网格体模拟物理
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None); // 停止移动
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld *World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(HitTimerHandle);
    }

    Super::EndPlay(EndPlayReason);
}

void ABaseCharacter::GiveDefaultAbility()
{
    if (!IsValid(GetAbilitySystemComponent()))
    {
        return;
    }
    
    for (const auto& Ability : GAClass)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
        GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
    }
}
