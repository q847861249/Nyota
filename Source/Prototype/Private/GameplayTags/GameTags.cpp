// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTags/GameTags.h"

namespace Nyota
{

// Ability
UE_DEFINE_GAMEPLAY_TAG(Ability_LightAttack, "Nyota.Ability.Attack.Light");
UE_DEFINE_GAMEPLAY_TAG(Ability_LeftLightAttack, "Nyota.Ability.Attack.LeftLight");
UE_DEFINE_GAMEPLAY_TAG(Ability_RightLightAttack, "Nyota.Ability.Attack.RightLight");
UE_DEFINE_GAMEPLAY_TAG(Ability_ActivateOnGive, "Nyota.Ability.ActivateOnGive");
UE_DEFINE_GAMEPLAY_TAG(Ability_WaterBall, "Nyota.Ability.WaterBall");
UE_DEFINE_GAMEPLAY_TAG(Ability_WaterBubble, "Nyota.Ability.WaterBubble");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab, "Nyota.Ability.Grab");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_Slam, "Nyota.Ability.Grab.Slam");

UE_DEFINE_GAMEPLAY_TAG(Ability_State, "Nyota.Ability.State")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing, "Nyota.Ability.State.Grabbing")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_Slam, "Nyota.Ability.State.Grabbing.Slam")

// Event
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_WaterBubbleEnd, "Nyota.Event.Ability.WaterBubbleEnd")

namespace Events::Enemy
{
UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GameTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(BoarLanded, "GameTags.Events.Boar.Landed", "Tag for the Boar Landed Event.");
} // namespace Events::Enemy
} // namespace Nyota
