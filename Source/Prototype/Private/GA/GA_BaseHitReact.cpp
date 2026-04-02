// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/GA_BaseHitReact.h"

#include "Character/BaseCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_BaseHitReact::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UE_LOG(LogTemp, Warning, TEXT("GA_BaseHitReact::ActivateAbility"));

    HitActors.Reset();

    AvatarCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
}

void UGA_BaseHitReact::EndAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    UE_LOG(LogTemp, Warning, TEXT("GA_BaseHitReact::EndAbility"));
}

void UGA_BaseHitReact::NiaAttach(UNiagaraSystem *NiagaraSystem, FVector SpawnLocation, FRotator SpawnRotation)
{
    if (!NiagaraSystem || !GetWorld())
    {
        return;
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(), NiagaraSystem, SpawnLocation, SpawnRotation, FVector(1.0f), true, true, ENCPoolMethod::AutoRelease
    );
}

void UGA_BaseHitReact::DelayAttack(
    ABaseCharacter *AttackCharacter, float AttackTime, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect,
    TSubclassOf<UGameplayEffect> HitEffectClass
)
{
    if (!AttackCharacter)
    {
        return;
    }

    UWorld *World = GetWorld();
    if (!World)
    {
        return;
    }

    // 如果没有延迟就直接攻击
    if (AttackTime <= 0.0f)
    {
        Attack(AttackCharacter, HitMontage, HitEffect, HitEffectClass);

        OnAttackFinished.Broadcast();

        return;
    }

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TWeakObjectPtr<UGA_BaseHitReact> WeakThis(this);

    TimerDelegate.BindLambda([WeakThis, AttackCharacter, HitMontage, HitEffect, HitEffectClass]() {
        if (WeakThis.IsValid())
        {
            WeakThis->Attack(AttackCharacter, HitMontage, HitEffect, HitEffectClass);

            WeakThis->OnAttackFinished.Broadcast();
        }
    });

    World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackTime, false);
}

void UGA_BaseHitReact::Attack(
    ABaseCharacter *AttackCharacter, UAnimMontage *HitMontage, UNiagaraSystem *HitEffect,
    TSubclassOf<UGameplayEffect> HitEffectClass
)
{
    if (!AttackCharacter)
    {
        return;
    }

    UWorld *World = GetWorld();
    if (!World)
    {
        return;
    }

    // 获取攻击者位置
    FVector StartLocation = AttackCharacter->GetActorLocation();

    // 球体检测半径
    float SphereRadius = 100.0f;

    // 设置要检测的碰撞通道
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    // 设置要忽略的Actor
    TArray<AActor *> ActorsToIgnore;
    ActorsToIgnore.Add(AttackCharacter);

    // 命中结果缓存
    TArray<FHitResult> HitResults;

    // 进行球体检测
    bool IsHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        World,
        StartLocation,
        StartLocation,
        SphereRadius,
        ObjectTypes,
        false,
        ActorsToIgnore,
        // EDrawDebugTrace::ForDuration,
        EDrawDebugTrace::None,
        HitResults,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        5.0f
    );

    if (!IsHit)
    {
        return;
    }

    // 处理命中结果
    for (const FHitResult &HitResult : HitResults)
    {
        AActor *HitActor = HitResult.GetActor(); // 获取命中的Actor
        if (!HitActor)
        {
            continue;
        }

        if (HitActors.Contains(HitActor))
        {
            continue;
        }
        HitActors.Add(HitActor); // 缓存命中的Actor

        // 如果命中的是敌人
        if (HitActor->ActorHasTag("Enemy"))
        {
            ABaseCharacter *EnemyCharacter = Cast<ABaseCharacter>(HitActor);
            if (!EnemyCharacter)
            {
                continue;
            }

            if (EnemyCharacter->bIsHit)
            {
                continue;
            }

            EnemyCharacter->PlayAnimMontage(HitMontage); // 播放命中动画

            EnemyCharacter->ApplyHealthChange(-1.0f); // 应用伤害

            UE_LOG(LogTemp, Warning, TEXT("Hit Enemy Current Health: %f"), EnemyCharacter->CurrentHealth);

            NiaAttach(
                HitEffect, EnemyCharacter->GetActorLocation(), EnemyCharacter->GetActorRotation()
            ); // 播放命中特效

            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
                EnemyCharacter->GetActorLocation(), AttackCharacter->GetActorLocation()
            );
            EnemyCharacter->SetActorRotation(LookAtRotation); // 设置敌人朝向攻击者

            EnemyCharacter->SetHit(); // 设置敌人被击中状态

            if (EnemyCharacter->CurrentHealth <= 0.0f)
            {
                EnemyCharacter->SetDead(); // 设置敌人死亡状态
            }
        }

        // 如果命中的是友军，TODO：播放命中动画和特效
        if (HitActor->ActorHasTag("Ally"))
        {
            ABaseCharacter *AllyCharacter = Cast<ABaseCharacter>(HitActor);
            if (!AllyCharacter)
            {
                continue;
            }

            AllyCharacter->PlayAnimMontage(HitMontage); // 播放命中动画

            UE_LOG(LogTemp, Warning, TEXT("AllyCharacter: %s, HitMontage: %s"), *AllyCharacter->GetName(), *HitMontage->GetName());

            NiaAttach(HitEffect, AllyCharacter->GetActorLocation(), AllyCharacter->GetActorRotation()); // 播放命中特效
        }
    }
}
