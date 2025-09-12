// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_Attack.h"
#include "Character/NyotaCharacters.h"
#include "Component/NyotaComponent.h"

#include "Weapon/NyotaWeapon.h"
#include "Input/NyotaGameplayTags.h"
#include "Weapon/NyotaShooter.h"
#include "Debug/Debug.h"
#include "Character/NyotaMovementComponent.h"
#include "Ability/NyotaAttributeSet.h"
#include "Ability/GT/Nyota_PlayMontageAndWaitForEvent.h"


UGA_Attack::UGA_Attack()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

bool UGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))  return false;
	
	const ANyotaCharacters* Character = CastChecked<ANyotaCharacters>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);

	const UNyotaComponent* component = CastChecked<UNyotaComponent>(Character->NyotaComponent, ECastCheckedType::NullAllowed);

	return true;
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);



	UAnimMontage** AnimMontage = LightAttackMontage.Find(CurrrentComboCount);

	if (!AnimMontage || !*AnimMontage) return;


	if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, *AnimMontage))
	{

		Task->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		Task->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		Task->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		Task->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);

		Task->ReadyForActivation();
	}

	if (CurrrentComboCount != LightAttackMontage.Num()) 
	{
		CurrrentComboCount += 1;
	}
	else 
	{
		CurrrentComboCount = 1;
	}

	//EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FNyotaGameplayTags::Get().State_Shooting, nullptr, true, true);

	//EventTask->EventReceived.AddDynamic(this, &UGA_Attack::shooting);

	//EventTask->Activate();


	//AActor* AvatarActor = GetAvatarActorFromActorInfo();
	//if (!AvatarActor)
	//{
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	//	return;
	//}

	//NyotaComponent = Cast<ANyotaCharacters>(AvatarActor)->NyotaComponent;
	//if (!NyotaComponent)
	//{
	//	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	//	return;

	//}
	//NyotaComponent->IncrementCombo();

	//UAnimMontage* Montage = GetNextComboMontage();

	////²¥·Å¹¥»÷¶¯»­

	//MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, getAvatarNyotaActor()->NyotaComponent->GetCurrentCombSequence());

	//UNyota_PlayMontageAndWaitForEvent* Task = UNyota_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, Montage, WaitForEventTag, Rate, NAME_None, true, 1.0f);


	//Task->OnBlendOut.AddDynamic(this, &UGA_Attack::OnMontageCompleted);
	//Task->OnCompleted.AddDynamic(this, &UGA_Attack::OnMontageCompleted);
	//Task->OnInterrupted.AddDynamic(this, &UGA_Attack::OnMontageCancelled);
	//Task->OnCancelled.AddDynamic(this, &UGA_Attack::OnMontageCancelled);
	//Task->EventReceived.AddDynamic(this, &UGA_Attack::OnEventReceived);

	//Task->ReadyForActivation();



}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

}

bool UGA_Attack::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}


void UGA_Attack::shooting(FGameplayEventData Payload)
{
	if (ANyotaShooter* shooter = Cast<ANyotaShooter>( getAvatarNyotaActor()->NyotaComponent->GetWeaponInstance()))
	{
		shooter->LaunchProjectile(FName()); 
	}
}






