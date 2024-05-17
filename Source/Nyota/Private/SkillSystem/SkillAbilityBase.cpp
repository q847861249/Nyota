// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystem/SkillAbilityBase.h"

USkillAbilityBase::USkillAbilityBase()
{
	//this->OwnerPawnBase = &StaticCast<APawnBase>(this->GetActorInfo().OwnerActor);
}

void USkillAbilityBase::LoadSkilConfig(FString& FileName)
{
}

bool USkillAbilityBase::Execute()
{
	return false;
}
