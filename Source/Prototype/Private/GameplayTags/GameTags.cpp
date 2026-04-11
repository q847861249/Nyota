// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTags/GameTags.h"

namespace GameTags
{
namespace Abilities
{
UE_DEFINE_GAMEPLAY_TAG_COMMENT(
    ActivateOnGive, "GameTags.Abilities.ActivateOnGive",
    "Tag for Abilities that should activate immediately once given."
);
}

namespace Events
{
namespace Enemy
{
UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GameTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event.");
}
} // namespace Events
} // namespace GameTags
