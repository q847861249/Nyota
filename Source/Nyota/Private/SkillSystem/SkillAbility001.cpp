// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystem/SkillAbility001.h"
#include "SkillSystem/Effect/Damage001EffectAbility.h"

//DEFINE_LOG_CATEGORY(SkillSystemLog)

USkillAbility001::USkillAbility001()
{
}
USkillAbility001* USkillAbility001::OnAnimNotifyOneParam(FString& NotifyName)
{
	return nullptr;
}

bool USkillAbility001::PreCall(FSkillParam* SklParam1)
{
	APawnBase* OwnerPawnBase1 = this->GetOwnerPawnBase();
	FString AnimName = FString("asd");
	FString NotifyName = FString("USkillAbility001");
	float NotifyTime = 1.f;
	//OwnerPawnBase1->PlayMontageAnim<USkillAbility001*, USkillAbility001, FString&>(this, &USkillAbility001::OnAnimNotifyOneParam, AnimName, NotifyName, NotifyTime);
	FAnimNotifyDelagateInfo<USkillAbility001*, USkillAbility001, FString&> AnimNotifyDelagateInfo;

	//获取法术场释放方向
	//this->SklCallParam->FirePosition;

	return false;
}

bool USkillAbility001::Invoke(FSkillCallParam* SklCallParam1)
{
	
	//播放动画TFunction<void()>

	auto b = MakeShared<TFunction<void()>>(
		(  /**/[&]()
			{
				// 假设你已经有了一个指向当前世界的指针  
				//UWorld* World = OwnerPawnBase1->GetWorld();
				/*AActor* EmiterActor = World->SpawnActor();*/
				//释放法术场
				APawnBase* OwnerPawnBase1 = this->GetOwnerPawnBase();
				OwnerPawnBase1->AbilitySystem->TryActivateAbilityByClass(OwnerPawnBase1->MyAbilities[1]);//(UDamage001EffectAbility::StaticClass());//
			})

	);

	APawnBase* OwnerPawnBase1 = this->GetOwnerPawnBase();
	OwnerPawnBase1->lambda_a = b.ToSharedPtr();//@note 这里要修改,不能把共享指针放栈中
	OwnerPawnBase1->EventTrigger->AddTimerTrigger(OwnerPawnBase1->lambda_a.Get()
	, OwnerPawnBase1->GetWorld(), 1.f);

	//绑定动画通知
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

