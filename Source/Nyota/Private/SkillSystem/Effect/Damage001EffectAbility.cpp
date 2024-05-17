// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystem/Effect/Damage001EffectAbility.h"
#include <AbilitySystemBlueprintLibrary.h>

void UDamage001EffectAbility::ActivateCppAbility()
{
	if (this->Direction == FVector(0,0,0))
	{
		UE_LOG(SkillSystemLog, Error, TEXT("UDamage001EffectAbility::ActivateCppAbility Error Param Have Not Init"));
		return;
	}
	this->MoveActor = UEffectAbility::CreateSphereEffectAbility(this, this->Scale, this->Position, this->Direction);
}


void UDamage001EffectAbility::EndCppAbility()
{
	if (this->MoveActor)
	{
		this->MoveActor->Destroy();
	}

}

void UDamage001EffectAbility::BeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	//自己排除在外
	if ((AActor*)this->GetOwnerPawnBase() == OtherActor)
	{
		return;
	}
	//拿到配置文件：伤害属性
	//根据EffectAblility文件名字，填充蓝图数组中对应的EffectAbility对象
	
	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, this->EffectNotifyTag, Payload);

	// 
	//effct对象应该根据标识触发接受伤害的BeAttackSkillAbly
	UE_LOG(SkillSystemLog, Log, TEXT("UDamage001EffectAbility::BeginOverlap Enter Name %s"), *OtherActor->GetName())
}