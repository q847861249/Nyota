// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/GA_BaseSkill.h"

#include "Character/BasePlayer.h"

UGA_BaseSkill::UGA_BaseSkill()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_BaseSkill::ActivateAbility(
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

FVector UGA_BaseSkill::GetAbilityDetectionDirection_Implementation() const
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