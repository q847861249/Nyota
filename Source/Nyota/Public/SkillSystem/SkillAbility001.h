// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillAbilityBase.h"

#include "SkillAbility001.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API USkillAbility001 : public USkillAbilityBase
{
	GENERATED_BODY()

public:
	USkillAbility001();

protected:

	virtual bool PreCall(FSkillParam* SklParam1) override;

	virtual bool Invoke(FSkillCallParam* SklCallParam1) override;

	virtual void LoadSkilConfig(FString& FileName) override;

	//执行技能流程
	virtual bool Execute() override;

	virtual void ActivateCppAbility() override;

public:
	//用GAS流程，完成能力释放过程
	//bool CanActive
	TSharedPtr<TFunction<void()>> lambda_a;

	USkillAbility001* OnAnimNotifyOneParam(FString& NotifyName);

	
};
