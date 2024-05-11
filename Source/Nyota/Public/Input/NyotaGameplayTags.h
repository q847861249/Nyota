// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


class UGameplayTagsManager;

struct FNyotaGameplayTags
{
public:

	static const FNyotaGameplayTags& Get() { return GameplayTags; }

	static const TArray<FGameplayTag>& GetAllTags() { return Array_GameplayTags; }

	static void InitializeNativeTags();


	//**输入标签**//
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Jump;



protected:

	//**使用 GameplayTags Manager 注册所有标签**//
	void AddAllTags(UGameplayTagsManager& Manager);

	//**添加标签**//
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

	//** 添加原有的标签，主要是蓝图原本保留的标签 **//
	void AddOriginTags();
private:

	static FNyotaGameplayTags GameplayTags;

	static TArray<FGameplayTag> Array_GameplayTags;
};
