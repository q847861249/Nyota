// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace Nyota
{

// Ability
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_LightAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_LeftLightAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_RightLightAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateOnGive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_WaterBall);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_WaterBubble);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab_Slam);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing_Slam)

// Event
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_WaterBubbleEnd)

namespace Events::Enemy
{
UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoarLanded);
} // namespace Events::Enemy
} // namespace Nyota
