// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_PickUp.h"
#include "AbilitySystemComponent.h"
#include "Character/BasePlayer.h"
#include "Actor/Coin.h"

// Destory the pick up Item and apply pick up effect to add score.
void UGA_PickUp::PickUpCoin(AActor *PickUpItem)
{
    ACoin *Coin = Cast<ACoin>(PickUpItem);
    Coin->Destroy();
    ABasePlayer *PC = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!PC)
    {
        return;
    }
    UAbilitySystemComponent *ASC = PC->GetAbilitySystemComponent();
    if (!ASC)
    {
        return;
    }

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PickUpCoinEffectClass, 1, ContextHandle);

    if (SpecHandle.IsValid())
    {
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
}