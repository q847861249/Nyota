// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"

#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"

UNyotaGameplayAbility::UNyotaGameplayAbility(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    ActivationPolicy = ENyotaAbilityActivationPolicy::OnInputTriggered;
}

void UNyotaGameplayAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (bDrawDebugs && IsValid(GEngine))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("%s Activated"), *GetName()));
    }
}

ENyotaAbilityActivationPolicy UNyotaGameplayAbility::GetActivatePolicy() const
{
    return ActivationPolicy;
}

FVector UNyotaGameplayAbility::GetAbilityDetectionDirection_Implementation() const
{
    AActor *AvatarActor = GetAvatarActorFromActorInfo();

    if (!IsValid(AvatarActor))
    {
        UE_LOG(LogTemp, Error, TEXT("AvatarActor is null."));

        return FVector::ForwardVector;
    }

    FVector BaseDirection = AvatarActor->GetActorForwardVector();

    ABaseCharacter *BaseCharacter = Cast<ABaseCharacter>(AvatarActor);
    if (IsValid(BaseCharacter))
    {
        BaseDirection = BaseCharacter->GetForwardDirection();
    }

    return BaseDirection;
}

void UNyotaGameplayAbility::ApplyCooldown(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo
) const
{
    if (!CooldownGameplayEffectClass)
    {
        return;
    }

    FGameplayEffectSpecHandle SpecHandle =
        MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, GetAbilityLevel());

    SpecHandle.Data->SetSetByCallerMagnitude(Nyota::CooldownDuration, CooldownDuration);

    ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}
