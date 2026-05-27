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
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_VortexGrip, "Nyota.Ability.Grab.VortexGrip");

UE_DEFINE_GAMEPLAY_TAG(Ability_State, "Nyota.Ability.State")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing, "Nyota.Ability.State.Grabbing")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_Slam, "Nyota.Ability.State.Grabbing.Slam")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_VortexGrip, "Nyota.Ability.State.Grabbing.VortexGrip")

UE_DEFINE_GAMEPLAY_TAG(Ability_init, "Nyota.Ability.init")

// Event
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_AttackStart, "Nyota.Event.Ability.AttackStart")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_AttackEnd, "Nyota.Event.Ability.AttackEnd")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_LightAttack, "Nyota.Event.Ability.LightAttack")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_ShootWaterBall, "Nyota.Event.Ability.ShootWaterBall")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_WaterBubbleEnd, "Nyota.Event.Ability.WaterBubbleEnd")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_StartGrabTrace, "Nyota.Event.Ability.StartGrabTrace")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_StopGrabTrace, "Nyota.Event.Ability.StopGrabTrace")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_GrabEnd, "Nyota.Event.Ability.GrabEnd")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_GrabSlam, "Nyota.Event.Ability.GrabSlam")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_GrabVortexGrip, "Nyota.Event.Ability.GrabVortexGrip")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_ApplyDamage, "Nyota.Event.Ability.ApplyDamage")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_HasGrabbedEnemy, "Nyota.Event.Ability.HasGrabbedEnemy")
UE_DEFINE_GAMEPLAY_TAG(Event_Enemy_HitReact, "Nyota.Event.Enemy.HitReact")
UE_DEFINE_GAMEPLAY_TAG(Event_Enemy_BoarLanded, "Nyota.Event.Enemy.BoarLanded")
UE_DEFINE_GAMEPLAY_TAG(Event_Item_PickUp, "Nyota.Event.Item.PickUp")

} // namespace Nyota
