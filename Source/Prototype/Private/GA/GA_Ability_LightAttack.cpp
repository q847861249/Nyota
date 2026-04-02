// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/GA_Ability_LightAttack.h"

#include "NiagaraSystem.h"
#include "Character/BaseCharacter.h"

void UGA_Ability_LightAttack::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AActor *AvatarActor = ActorInfo->AvatarActor.Get();
    ABaseCharacter *OwnerCharacter = Cast<ABaseCharacter>(AvatarActor);
    if (!OwnerCharacter || !OwnerCharacter->GetController())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // 调整玩家朝向目标
    FRotator NewControlRotation = OwnerCharacter->GetController()->GetControlRotation() + FRotator(0, -90, 0);
    NewControlRotation.Pitch = 0.0f;
    NewControlRotation.Roll = 0.0f;
    OwnerCharacter->SetActorRotation(NewControlRotation);

    // 播放攻击动画
    UAnimInstance *AnimInstance = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
    if (AnimInstance && AttackMontage)
    {
        AnimInstance->Montage_Play(AttackMontage);
    }

    // 绑定攻击完成回调
    TWeakObjectPtr<UGA_Ability_LightAttack> WeakThis(this);
    OnAttackFinished.AddLambda([WeakThis, Handle, CacheActorInfo = *ActorInfo, ActivationInfo] {
        if (WeakThis.IsValid())
        {
            WeakThis->EndAbility(Handle, &CacheActorInfo, ActivationInfo, true, false);
        }
    });

    // 执行攻击
    DelayAttack(OwnerCharacter, AttackTime, TargetHitReactMontage, CastNiagaraEffect, HitEffectClass);
}

void UGA_Ability_LightAttack::EndAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
