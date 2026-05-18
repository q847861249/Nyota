// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseEnemyWildBoar.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "GameplayTags/GameTags.h"

void ABaseEnemyWildBoar::OnGrabbed(ACharacter *Grabber)
{
    bIsGrabbed = true;

    // 禁用 AI
    if (AAIController *AIC = Cast<AAIController>(GetController()))
    {
        AIC->StopMovement();
        AIC->SetActorTickEnabled(false);
    }

    // 禁用移动和碰撞
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Attach 到螃蟹的 GrabSocket
    AttachToComponent(Grabber->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GrabSocket"));

    // 监听落地事件
    LandedDelegate.AddDynamic(this, &ThisClass::OnBoarLanded);

    // 通知蓝图播放表现
    OnGrabbed_Visual();
}

void ABaseEnemyWildBoar::OnReleased()
{
    bIsGrabbed = false;

    // Detach
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // 恢复碰撞和移动
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    // 恢复 AI
    if (AAIController *AIC = Cast<AAIController>(GetController()))
    {
        AIC->SetActorTickEnabled(true);
    }

    LandedDelegate.RemoveDynamic(this, &ThisClass::OnBoarLanded);
}

void ABaseEnemyWildBoar::OnThrown(FVector Direction, float Force)
{
    bIsGrabbed = false;

    // Detach
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    // 恢复碰撞
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // 切换到飞行状态
    GetCharacterMovement()->SetMovementMode(MOVE_Falling);

    // 施加力
    FVector LaunchVelocity = Direction * Force;
    LaunchVelocity.Z += ThrownZOffset;
    LaunchCharacter(LaunchVelocity, true, true);

    // 恢复 AI
    if (AAIController *AIC = Cast<AAIController>(GetController()))
    {
        AIC->SetActorTickEnabled(true);
    }

    // 通知蓝图播放表现
    OnThrown_Visual();
}

void ABaseEnemyWildBoar::OnBoarLanded(const FHitResult &Hit)
{
    if (bIsGrabbed)
    {
        return;
    }

    // 发送落地事件，使用 GE 处理伤害
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Nyota::Event_Enemy_BoarLanded, FGameplayEventData());

    // 通知蓝图播放落地特效
    OnLanded_Visual();

    LandedDelegate.RemoveDynamic(this, &ThisClass::OnBoarLanded);
}

void ABaseEnemyWildBoar::OnSlammed(int32 SlamCount)
{
}