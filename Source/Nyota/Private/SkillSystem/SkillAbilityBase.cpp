// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystem/SkillAbilityBase.h"

USkillAbilityBase::USkillAbilityBase()
{
	//this->OwnerPawnBase = StaticCast<APawnBase*>(this->GetActorInfo().OwnerActor.Get());
}

void USkillAbilityBase::LoadSkilConfig(FString& FileName)
{
}

bool USkillAbilityBase::Execute()
{
	return false;
}

APawnBase* USkillAbilityBase::GetOwnerPawnBase()
{
 	if (!this->OwnerPawnBase)
	{
		this->OwnerPawnBase = StaticCast<APawnBase*>(this->GetActorInfo().OwnerActor.Get());
		return this->OwnerPawnBase;
	}
	return this->OwnerPawnBase;
}

void USkillAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//保持父方法流程正常执行，并加入c++流程
	if (TriggerEventData && bHasBlueprintActivateFromEvent)
	{
		return;
	}
	if (bHasBlueprintActivate)
	{
		this->ActivateCppAbility();
		UE_LOG(SkillSystemLog, Log, TEXT("USkillAbilityBase::ActivateAbility %s Blueprint Actived"), *this->GetPathName());
	}
	else
	{
		UE_LOG(SkillSystemLog, Warning, TEXT("USkillAbilityBase::ActivateAbility %s Blueprint Has Not Actived"), *this->GetPathName());
	}

}

void USkillAbilityBase::ActivateCppAbility()
{
}

void USkillAbilityBase::EndCppAbility()
{
}
