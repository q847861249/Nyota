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


	//**�����ǩ**//
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Jump;



protected:

	//**ʹ�� GameplayTags Manager ע�����б�ǩ**//
	void AddAllTags(UGameplayTagsManager& Manager);

	//**��ӱ�ǩ**//
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

	//** ���ԭ�еı�ǩ����Ҫ����ͼԭ�������ı�ǩ **//
	void AddOriginTags();
private:

	static FNyotaGameplayTags GameplayTags;

	static TArray<FGameplayTag> Array_GameplayTags;
};
