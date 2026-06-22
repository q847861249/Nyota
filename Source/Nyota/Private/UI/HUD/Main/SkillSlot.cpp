// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/SkillSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "DataAsset/PlayerCharacterDataAsset.h"
void USkillSlot::UpdateCooldownUI()
{
    if(AbilityRemainTime > 0.f)
    {
        CooldownText->SetText(FText::AsNumber(FMath::CeilToInt(AbilityRemainTime)));
        Mask->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        CooldownText->SetText(FText::GetEmpty());
        Mask->SetVisibility(ESlateVisibility::Collapsed);
    }
}
bool USkillSlot::GetAbilityCoolDown()
{
    FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
    if (!Spec || !Spec->Ability)
    {
        UE_LOG(LogTemp,Warning,TEXT("没获得Spec"));
        return false;
    }

    const FGameplayAbilityActorInfo* ActorInfo = ASC->AbilityActorInfo.Get();
    if (!ActorInfo)
    {
        UE_LOG(LogTemp,Warning,TEXT("没获得ActorInfo"));
        return false;
    }

    Spec->Ability->GetCooldownTimeRemainingAndDuration(
        Spec->Handle,
        ActorInfo,
        AbilityRemainTime,
        AbilityDuration
    );
    UE_LOG(LogTemp,Warning,TEXT("Ability:%s,RemainTime: %f"),*Spec->Ability->GetName(),AbilityRemainTime);
    return true;
}

void USkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (GetAbilityCoolDown())
    {
        UpdateCooldownUI();
    }

}
void USkillSlot::Init(UAbilitySystemComponent* OutASC,FSkillUIData SkillData)
{
    ASC = OutASC;
    Icon->SetBrushFromTexture(SkillData.AbilityIcon);
    AbilityClass = SkillData.AbilityClass;

}