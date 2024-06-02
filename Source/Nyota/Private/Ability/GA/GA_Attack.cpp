// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GA/GA_Attack.h"
#include "Character/NyotaCharacters.h"
#include "Component/NyotaComponent.h"
#include "Weapon/NyotaShooter.h"
#include "Input/NyotaGameplayTags.h"


UGA_Attack::UGA_Attack()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{




	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ANyotaShooter* shooter = Cast<ANyotaShooter>(getAvatarNyotaActor()->NyotaComponent->GetWeaponInstance());

	if (!shooter) {
		K2_EndAbility();
		return;
	}
	//¹¥»÷¶¯»­
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, shooter->AttackAnimMontage);
	MontageTask->ReadyForActivation();

	MontageTask->OnBlendOut.AddDynamic(this, &UGA_Attack::K2_EndAbility);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_Attack::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_Attack::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Attack::K2_EndAbility);

	//·¢Éäµ¯µÀ
	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FNyotaGameplayTags::Get().State_Shooting,nullptr,true,true);

	EventTask->EventReceived.AddDynamic(this, &UGA_Attack::shooting);

	EventTask->Activate();
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//clear the task
	MontageTask->EndTask();
	EventTask->EndTask();


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_Attack::shooting(FGameplayEventData Payload)
{
	if (ANyotaShooter* shooter = Cast<ANyotaShooter>( getAvatarNyotaActor()->NyotaComponent->GetWeaponInstance())){

		shooter->LaunchProjectile(FName()); 

		K2_EndAbility();
	}
}


