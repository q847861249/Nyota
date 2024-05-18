// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystem/SkillAbility001.h"
#include "SkillSystem/Effect/Damage001EffectAbility.h"

USkillAbility001::USkillAbility001()
{
}
USkillAbility001* USkillAbility001::OnAnimNotifyOneParam(FString& NotifyName)
{
	return nullptr;
}

void USkillAbility001::OnTimerNotify(FString* NotifyName)
{
	APawnBase* OwnerPawnBase1 = this->GetOwnerPawnBase();
	OwnerPawnBase1->AbilitySystem->TryActivateAbilityByClass(OwnerPawnBase1->MyAbilities[1]);
}

bool USkillAbility001::PreCall(FSkillParam* SklParam1)
{
	APawnBase* OwnerPawnBase1 = this->GetOwnerPawnBase();
	FString AnimName = FString("asd");
	FString NotifyName = FString("USkillAbility001");
	float NotifyTime = 1.f;
	//OwnerPawnBase1->PlayMontageAnim<USkillAbility001*, USkillAbility001, FString&>(this, &USkillAbility001::OnAnimNotifyOneParam, AnimName, NotifyName, NotifyTime);
	FAnimNotifyDelagateInfo<USkillAbility001*, USkillAbility001, FString&> AnimNotifyDelagateInfo;

	//��ȡ�������ͷŷ���
	//this->SklCallParam->FirePosition;

	return false;
}

bool USkillAbility001::Invoke(FSkillCallParam* SklCallParam1)
{
	
	//���Ŷ���TFunction<void()>

	auto b = MakeShared<TFunction<void()>>(
		(  /**/[&]()
			{
				// �������Ѿ�����һ��ָ��ǰ�����ָ��  
				//UWorld* World = OwnerPawnBase1->GetWorld();
				/*AActor* EmiterActor = World->SpawnActor();*/
				//�ͷŷ�����
				APawnBase* OwnerPawnBase1 = this->GetOwnerPawnBase();
				OwnerPawnBase1->AbilitySystem->TryActivateAbilityByClass(OwnerPawnBase1->MyAbilities[1]);
			})

	);
	
	APawnBase* OwnerPawnBase1 = this->GetOwnerPawnBase();

	//�¼�������
	TMulticastDelegate<void(FString*)> as;
	TMulticastDelegate<void(FString*)>* NotifyDel = new TMulticastDelegate<void(FString*)>(); //StaticCast<TMulticastDelegate<void(FString*)>*>(FMemory::Malloc(sizeof(as.GetAllocatedSize())));
	NotifyDel->AddUObject(this, &USkillAbility001::OnTimerNotify);
	OwnerPawnBase1->EventTriggerObj->AddTimerTrigger(NotifyDel
	, OwnerPawnBase1->GetWorld(), 0.5f);

	//�󶨶���֪ͨ
	return true;
}

void USkillAbility001::LoadSkilConfig(FString& FileName)
{
}

bool USkillAbility001::Execute()
{
	return false;
}

void USkillAbility001::ActivateCppAbility()
{
	this->Invoke(nullptr);
}

