// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Mmo/PawnBase.h"

#include "SkillAbilityBase.generated.h"

USTRUCT()
struct FSkillParam
{
	GENERATED_BODY()
public: 
	FVector IsHavePreCall;
	FVector FirePosition;
	FVector FireRotation;
	APawnBase* Possessor;
};

USTRUCT()
struct FSkillCallParam
{
	GENERATED_BODY()
public:
	FVector IsHavePreCall;
	FVector FirePosition;
	FVector FireRotation;
	APawnBase* Possessor;
};

/**
 * 
 */
UCLASS()
class NYOTA_API USkillAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USkillAbilityBase();
	
public:
	//ǰҡ�߼�
	virtual bool PreCall(FSkillParam* SklParam1) { return false; }
	
	//��ʽ�ͷż��ܶ���
	virtual bool Invoke(FSkillCallParam* SklCallParam1) { return false; }

	virtual void LoadSkilConfig(FString& FileName);

	//ִ�м�������
	virtual bool Execute();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	//GAS��ܼ���Cpp�ص�����
	virtual void ActivateCppAbility();

	//ʧ��
	virtual void EndCppAbility();
public:

	//���ܲ���
	FSkillParam* SklParam;
	FSkillCallParam* SklCallParam;

	APawnBase* OwnerPawnBase;

	APawnBase* GetOwnerPawnBase();
};
