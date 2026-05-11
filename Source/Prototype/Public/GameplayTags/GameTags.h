// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace Nyota
{

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateOnGive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_WaterBall);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab_Slam);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing_Slam)

namespace Events::Enemy
{
UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(BoarLanded);
} // namespace Events::Enemy
} // namespace Nyota
