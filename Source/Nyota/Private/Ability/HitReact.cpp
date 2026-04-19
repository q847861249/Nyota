// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/HitReact.h"
#include "Tags/Nyota_Tag.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Characters/BaseCharacter.h"
#include "AttributeSet/Nyota_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
// acording to the data of damage ability to apply damage Effect.
void UHitReact::ApplyDamage(TSubclassOf<UGameplayEffect> GEClass, float Damage,AActor* OutInstigator)
{
    ABaseCharacter* BC = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if(!BC) return;
    ASC = BC->GetAbilitySystemComponent();
    if(!ASC) return;
    Instigator = Cast<ABaseCharacter>(OutInstigator);
    FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GEClass,1.0,Context);

    if(SpecHandle.IsValid())
    {
        SpecHandle.Data.Get()->SetSetByCallerMagnitude(Nyota::Data::Damage,Damage);
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

}

void UHitReact::ListenHealth()
{
    ABaseCharacter* BC = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if(!BC) return;
    ASC = BC->GetAbilitySystemComponent();
    if(!ASC) return;
    ASC->GetGameplayAttributeValueChangeDelegate(UNyota_AttributeSet::GetHealthAttribute())
    .AddUObject(this,&UHitReact::OnHealthValueChange);
}

void UHitReact::OnHealthValueChange(const struct FOnAttributeChangeData& Data)
{
    if(!ASC) return;
    float CurrentHealth = ASC->GetNumericAttribute(UNyota_AttributeSet::GetHealthAttribute());
    if(CurrentHealth == 0)
    {
        FGameplayEventData Payload;
        Payload.Instigator = Instigator;
        float Reward = ASC->GetNumericAttribute(UNyota_AttributeSet::GetScoreAttribute());
        Payload.EventMagnitude = Reward / DeathDropRate;
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator,Nyota::Event::Kill,Payload);
        FGameplayTagContainer TagContainer;
        TagContainer.AddTag(Nyota::Ability::Death);
        ASC->TryActivateAbilitiesByTag(TagContainer);
    }
}

