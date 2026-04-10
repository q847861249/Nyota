// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Player/PickUpAbility.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameItems/PickUpItem/Coin.h"

void UPickUpAbility::PickUpCoin(AActor *PickUpItem)
{
    ACoin* Coin = Cast<ACoin>(PickUpItem);
    Coin->Destroy();
    APlayerCharacter* PC = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
    if(!PC) return;
    UAbilitySystemComponent* ASC = PC->GetAbilitySystemComponent();
    if(!ASC) return;

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(PickUpCoinEffectClass,1,ContextHandle);

    if(SpecHandle.IsValid())
    {
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
}