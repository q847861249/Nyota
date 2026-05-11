// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTags/GameTags.h"

namespace Nyota
{

UE_DEFINE_GAMEPLAY_TAG(Ability_ActivateOnGive, "Nyota.Ability.ActivateOnGive");
UE_DEFINE_GAMEPLAY_TAG(Ability_WaterBall, "Nyota.Ability.WaterBall");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab, "Nyota.Ability.Grab");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_Slam, "Nyota.Ability.Grab.Slam");

UE_DEFINE_GAMEPLAY_TAG(Ability_State, "Nyota.Ability.State")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing, "Nyota.Ability.State.Grabbing")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_Slam, "Nyota.Ability.State.Grabbing.Slam")

namespace Events::Enemy
{
UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GameTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoarLanded, "GameTags.Events.Boar.Landed", "Tag for the Boar Landed Event.");
} // namespace Events::Enemy
} // namespace Nyota
