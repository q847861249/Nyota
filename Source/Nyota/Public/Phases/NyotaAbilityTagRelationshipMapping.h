// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "NyotaAbilityTagRelationshipMapping.generated.h"

USTRUCT()
struct FNyotaAbilityTagRelationship
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Ability)
    FGameplayTag AbilityTag;

    // 使用当前 Tag 的 Ability 激活时，会阻止（Block）的其他 Ability Tag。
    UPROPERTY(EditAnywhere, Category = Ability)
    FGameplayTagContainer AbilityTagsToBlock;

    // 任何拥有当前 Tag 的 Ability 激活时，将取消（Cancel）拥有这些 Tag 的其他 Ability。
    UPROPERTY(EditAnywhere, Category = Ability)
    FGameplayTagContainer AbilityTagsToCancel;

    // 当 Ability 拥有当前 Tag 时，它会自动额外要求具备这些 Tag 才能被激活。
    UPROPERTY(EditAnywhere, Category = Ability)
    FGameplayTagContainer ActivationRequiredTags;

    // 当 Ability 拥有当前 Tag 时，只要角色拥有这些 Tag，该 Ability 就无法激活。
    UPROPERTY(EditAnywhere, Category = Ability)
    FGameplayTagContainer ActivationBlockedTags;
};

/**
 *
 */
UCLASS()
class NYOTA_API UNyotaAbilityTagRelationshipMapping : public UDataAsset
{
    GENERATED_BODY()

public:
    /**
     * @brief Given a set of ability tags, parse the tag relationship and fill out tags to block and cancel
     * @param AbilityTags
     * @param OutTagsToBlock
     * @param OutTagsToCancel
     */
    void GetAbilityTagsToBlockAndCancel(
        const FGameplayTagContainer &AbilityTags, FGameplayTagContainer *OutTagsToBlock,
        FGameplayTagContainer *OutTagsToCancel
    ) const;

    /**
     * @brief Given a set of ability tags, add additional required and blocking tags
     * @param AbilityTags
     * @param OutActivationRequired
     * @param OutActivationBlocked
     */
    void GetRequiredAndBlockedActivationTags(
        const FGameplayTagContainer &AbilityTags, FGameplayTagContainer *OutActivationRequired,
        FGameplayTagContainer *OutActivationBlocked
    ) const;

    /**
     * @brief Returns true if the specified ability tags are canceled by the passed in action tag
     * @param AbilityTags
     * @param ActionTag
     * @return
     */
    bool IsAbilityCancelByTag(const FGameplayTagContainer &AbilityTags, const FGameplayTag &ActionTag) const;

private:
    // 定义 Gameplay Tag 之间相互关系的配置列表，包括哪些 Tag 会阻塞（Block）或取消（Cancel）其他 Tag。
    UPROPERTY(EditAnywhere, Category = Ability, meta = (TitleProperty = "AbilityTag"))
    TArray<FNyotaAbilityTagRelationship> AbilityTagRelationships;
};
