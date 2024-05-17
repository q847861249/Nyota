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
	//前摇逻辑
	virtual bool PreCall(FSkillParam* SklParam1) { return false; }
	
	//正式释放技能动作
	virtual bool Invoke(FSkillCallParam* SklCallParam1) { return false; }

	virtual void LoadSkilConfig(FString& FileName);

	//执行技能流程
	virtual bool Execute();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	//GAS框架加入Cpp回调激活
	virtual void ActivateCppAbility();

	//失活
	virtual void EndCppAbility();
public:

	//技能参数
	FSkillParam* SklParam;
	FSkillCallParam* SklCallParam;

	APawnBase* OwnerPawnBase;

	APawnBase* GetOwnerPawnBase();
};
