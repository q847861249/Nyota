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

	//�Լ��ų�����
	if ((AActor*)this->GetOwnerPawnBase() == OtherActor)
	{
		return;
	}
	//�õ������ļ����˺�����
	//����EffectAblility�ļ����֣������ͼ�����ж�Ӧ��EffectAbility����
	
	FGameplayEventData Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, this->EffectNotifyTag, Payload);

	// 
	//effct����Ӧ�ø��ݱ�ʶ���������˺���BeAttackSkillAbly
	UE_LOG(SkillSystemLog, Log, TEXT("UDamage001EffectAbility::BeginOverlap Enter Name %s"), *OtherActor->GetName())
}