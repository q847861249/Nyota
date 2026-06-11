// Fill out your copyright notice in the Description page of Project Settings.

#include "Phases/NyotaAbilityTagRelationshipMapping.h"

void UNyotaAbilityTagRelationshipMapping::GetAbilityTagsToBlockAndCancel(
    const FGameplayTagContainer &AbilityTags, FGameplayTagContainer *OutTagsToBlock,
    FGameplayTagContainer *OutTagsToCancel
) const
{
    for (int32 i = 0; i < AbilityTagRelationships.Num(); i++)
    {
        const FNyotaAbilityTagRelationship &AbilityTagRelationship = AbilityTagRelationships[i];
        if (!AbilityTags.HasTag(AbilityTagRelationship.AbilityTag))
        {
            continue;
        }

        if (OutTagsToBlock)
        {
            OutTagsToBlock->AppendTags(AbilityTagRelationship.AbilityTagsToBlock);
        }

        if (OutTagsToCancel)
        {
            OutTagsToCancel->AppendTags(AbilityTagRelationship.AbilityTagsToCancel);
        }
    }
}

void UNyotaAbilityTagRelationshipMapping::GetRequiredAndBlockedActivationTags(
    const FGameplayTagContainer &AbilityTags, FGameplayTagContainer *OutActivationRequired,
    FGameplayTagContainer *OutActivationBlocked
) const
{
    for (int32 i = 0; i < AbilityTagRelationships.Num(); i++)
    {
        const FNyotaAbilityTagRelationship &AbilityTagRelationship = AbilityTagRelationships[i];
        if (!AbilityTags.HasTag(AbilityTagRelationship.AbilityTag))
        {
            continue;
        }

        if (OutActivationRequired)
        {
            OutActivationRequired->AppendTags(AbilityTagRelationship.ActivationRequiredTags);
        }

        if (OutActivationBlocked)
        {
            OutActivationBlocked->AppendTags(AbilityTagRelationship.ActivationBlockedTags);
        }
    }
}

bool UNyotaAbilityTagRelationshipMapping::IsAbilityCancelByTag(
    const FGameplayTagContainer &AbilityTags, const FGameplayTag &ActionTag
) const
{
    for (int32 i = 0; i < AbilityTagRelationships.Num(); i++)
    {
        const FNyotaAbilityTagRelationship &AbilityTagRelationship = AbilityTagRelationships[i];

        if (AbilityTagRelationship.AbilityTag == ActionTag &&
            AbilityTagRelationship.AbilityTagsToCancel.HasAny(AbilityTags))
        {
            return true;
        }
    }

    return false;
}