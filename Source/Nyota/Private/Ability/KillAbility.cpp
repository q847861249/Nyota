// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/KillAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Characters/BaseCharacter.h"
#include "Tags/Nyota_Tag.h"
void UKillAbility::ApplyKill(TSubclassOf<UGameplayEffect> KillEffClass,float KillReward)
{
    ABaseCharacter* BC = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if(!BC) return;
    UAbilitySystemComponent* ASC = BC->GetAbilitySystemComponent();
    if(!ASC) return;
    FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(KillEffClass,1.0,Context);

    if(SpecHandle.IsValid())
    {
        SpecHandle.Data.Get()->SetSetByCallerMagnitude(Nyota::Data::KillReward,KillReward);
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
}