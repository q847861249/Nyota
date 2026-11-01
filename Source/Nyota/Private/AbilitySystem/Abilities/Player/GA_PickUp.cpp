// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_PickUp.h"
#include "AbilitySystemComponent.h"
#include "Character/BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Actor/Loot.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
void UGA_PickUp::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    ABasePlayer* Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if(!Player) return;
    UCapsuleComponent* CapsuleComponent = Player->GetCapsuleComponent();
    if(!CapsuleComponent) return;
    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnOverlapBegin);
}
void UGA_PickUp::OnOverlapBegin(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult
)
{
    // Check if the actor has specific tag
    if (OtherActor && OtherActor->ActorHasTag(FName("CanPickUp")))
    {
        PickUpLoot(OtherActor);
    }
}
void UGA_PickUp::PickUpLoot(AActor* Actor)
{
    ALoot* Loot = Cast<ALoot>(Actor);
    if(!Loot) return;
    CurrentLoot = Loot;
    Orientation();
    Channeling();

    // Loot->Destory();
    // //apply Effect
    // ABasePlayer *PC = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    // if (!PC) return;
    // UAbilitySystemComponent *ASC = PC->GetAbilitySystemComponent();
    // if (!ASC) return;
    // FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    // FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PickUpEffectClass, 1, ContextHandle);
    // if (SpecHandle.IsValid())
    // {
    //     ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    // }

}
void UGA_PickUp::Channeling()
{
    if(!IsValid(PickUpMontage)) return;
    UAbilityTask_PlayMontageAndWait *PlayMontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PickUp"), PickUpMontage);
    PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnPickUpComplete);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::RestorePlayerInput);
    PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::RestorePlayerInput);
    PlayMontageTask->ReadyForActivation();
}

void UGA_PickUp::EndAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
void UGA_PickUp::Orientation()
{
    ABasePlayer* Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!Player) return;
    //caculate orientation vector
    FVector Direction = CurrentLoot->GetActorLocation() - Player->GetActorLocation();
    // ignore Z value. preventing head tilt
    Direction.Z = 0.f;
    // GetForwardDirection()
    if (!Direction.IsNearlyZero())
    {
        const FRotator TargetRotation = Direction.Rotation();
 
        Player->SetActorRotation(FRotator(0.f,TargetRotation.Yaw,0.f));
    }

    // 屏蔽移动输入
    if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
    {
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
    }

}
void UGA_PickUp::RestorePlayerInput()
{
    ABasePlayer* Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!Player)return;
    if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
    {
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
    }
}
void UGA_PickUp::CleanPickUpUI()
{
    UE_LOG(LogTemp,Warning,TEXT("啊啊啊啊"));
}
void UGA_PickUp::OnPickUpComplete()
{
    if(!IsValid(CurrentLoot)) return;
    UE_LOG(LogTemp,Warning,TEXT("触发啊"));
    CurrentLoot->Destory();
    //apply Effect
    ABasePlayer *PC = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!PC) return;
    UAbilitySystemComponent *ASC = PC->GetAbilitySystemComponent();
    if (!ASC) return;
    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PickUpEffectClass, 1, ContextHandle);
    if (SpecHandle.IsValid())
    {
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
    RestorePlayerInput();
}