// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/HitReact.h"
#include "Tags/Nyota_Tag.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Characters/BaseCharacter.h"

// acording to the data of damage ability to apply damage Effect.
void UHitReact::ApplyDamage(TSubclassOf<UGameplayEffect> GEClass, float Damage)
{
    ABaseCharacter* BC = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if(!BC) return;
    UAbilitySystemComponent* ASC = BC->GetAbilitySystemComponent();
    if(!ASC) return;
    FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GEClass,1.0,Context);

    if(SpecHandle.IsValid())
    {
        SpecHandle.Data.Get()->SetSetByCallerMagnitude(Nyota::Ability::Damage,Damage);
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

}