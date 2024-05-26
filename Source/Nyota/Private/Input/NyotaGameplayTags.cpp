// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/NyotaGameplayTags.h"
#include "GameplayTagsManager.h"

FNyotaGameplayTags FNyotaGameplayTags::GameplayTags;
TArray<FGameplayTag> FNyotaGameplayTags::Array_GameplayTags;

void FNyotaGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

}

void FNyotaGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddOriginTags();

	//character movement tag
	AddTag(InputTag_Move, "InputTag.Move", "Move input.");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	AddTag(InputTag_Jump, "InputTag.Jump", "Jumping");
	AddTag(InputTag_Attack, "InputTag.Attack", "BasicAttack");
	
	//character state tag
	AddTag(State_Attack, "State.Attack", "State Attack.");
	AddTag(State_OnHit, "State.OnHit", "State OnHit.");
	AddTag(State_Dead, "State.Dead", "State Dead.");

}

void FNyotaGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));

	Array_GameplayTags.Add(OutTag);
}

void FNyotaGameplayTags::AddOriginTags()
{
	FGameplayTagContainer TagContainer;
	TArray<FGameplayTag> Tags;

	TagContainer.GetGameplayTagArray(Tags);

	for (FGameplayTag Tag : Tags)
	{
		Array_GameplayTags.Add(Tag);
	}
}
