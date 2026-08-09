// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_PickUp.h"
#include "AbilitySystemComponent.h"
#include "Character/BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Actor/Loot.h"
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
    Channeling();
    Loot->Destory();
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
}
void UGA_PickUp::Channeling()
{
}
