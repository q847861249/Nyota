// Fill out your copyright notice in Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_ChargeLightAttack.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Player/GA_Charge.h"
#include "Character/BaseCharacter.h"
#include "GameplayTags/GameTags.h"

UGA_ChargeLightAttack::UGA_ChargeLightAttack(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    AbilityTags.AddTag(Nyota::Ability_Charge_LightAttack);

    // 只有处于推土机状态（冲锋撞到人）才允许激活，对应设计中的"派生窗口：推人时可派生普攻"
    ActivationRequiredTags.AddTag(Nyota::Ability_State_Charging_Bulldozer);
}

void UGA_ChargeLightAttack::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UGA_Charge *ChargeAbility = FindActiveChargeAbility();

    if (!IsValid(ChargeAbility))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

        return;
    }

    ABaseCharacter *BulldozedEnemy = ChargeAbility->GetBulldozedEnemy();

    if (!IsValid(BulldozedEnemy))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

        return;
    }

    OnLaunchEnemy_Visual(BulldozedEnemy);

    // 顶飞敌人并结束冲锋（LaunchBulldozedEnemy 内部会调用 FinishCharge 结束冲锋技能）
    ChargeAbility->LaunchBulldozedEnemy(BulldozedEnemy);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

UGA_Charge *UGA_ChargeLightAttack::FindActiveChargeAbility() const
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponentFromActorInfo();

    if (!IsValid(ASC))
    {
        return nullptr;
    }

    for (const FGameplayAbilitySpec &Spec : ASC->GetActivatableAbilities())
    {
        if (!Spec.IsActive() || !IsValid(Spec.Ability))
        {
            continue;
        }

        if (!Spec.Ability->AbilityTags.HasTagExact(Nyota::Ability_Charge))
        {
            continue;
        }

        PRAGMA_DISABLE_DEPRECATION_WARNINGS
        return Cast<UGA_Charge>(Spec.GetPrimaryInstance());
        PRAGMA_ENABLE_DEPRECATION_WARNINGS
    }

    return nullptr;
}
