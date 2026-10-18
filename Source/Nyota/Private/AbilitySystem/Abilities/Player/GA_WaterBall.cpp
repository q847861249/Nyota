// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_WaterBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Actor/BaseProjectile.h"
#include "Character/BaseCharacter.h"
#include "GameplayTags/GameTags.h"

void UGA_WaterBall::ActivateAbility(
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

    Execute();
}

void UGA_WaterBall::Execute()
{

    UAbilityTask_PlayMontageAndWait *PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this, FName("ShootWaterBall"), ShootWaterBallMontage
    );
    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageCompleted);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCompleted);
    PlayMontageTask->ReadyForActivation();

    // Wait Event
    UAbilityTask_WaitGameplayEvent *WaitGameplayEventTask =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, Nyota::Event_Ability_ShootWaterBall);
    WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnWaitGameplayEvent);
    WaitGameplayEventTask->ReadyForActivation();
}

void UGA_WaterBall::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_WaterBall::OnWaitGameplayEvent(FGameplayEventData EventData)
{
    SpawnWaterBall();
}

void UGA_WaterBall::SpawnWaterBall()
{
    ABaseCharacter *BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if (!IsValid(BaseCharacter))
    {
        return;
    }

    UWorld *World = GetWorld();
    if (!IsValid(GetWorld()))
    {
        return;
    }

    FVector FireLocation = BaseCharacter->GetMesh()->GetSocketLocation(FireSockName);
    FRotator ControlRotation = BaseCharacter->GetControlRotation();
    FTransform SpawnTransform(ControlRotation, FireLocation, FVector(1.0f, 1.0f, 1.0f));

    FActorSpawnParameters Params;
    Params.Owner = BaseCharacter;
    Params.Instigator = BaseCharacter;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

    ABaseProjectile *WaterBallActor = World->SpawnActor<ABaseProjectile>(WaterBallClass, SpawnTransform, Params);
    WaterBallActor->OnProjectileHit.AddDynamic(this, &UGA_WaterBall::OnWaterBallHit);
}

void UGA_WaterBall::OnWaterBallHit(const FHitResult &HitResult)
{
    ABaseCharacter *HitActor = Cast<ABaseCharacter>(HitResult.GetActor());
    if (!IsValid(HitActor))
    {
        return;
    }

    if (!IsValid(HitActor->GetAbilitySystemComponent()))
    {
        return;
    }

    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayEventData PayloadData;
    PayloadData.Instigator = GetAvatarActorFromActorInfo();
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, Nyota::Event_Enemy_HitReact, PayloadData);

    FGameplayEffectSpecHandle SpecHandle =
        ASC->MakeOutgoingSpec(HitEffectClass, GetAbilityLevel(), FGameplayEffectContextHandle());
    ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitActor->GetAbilitySystemComponent());
}