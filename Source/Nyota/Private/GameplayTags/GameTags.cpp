// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayTags/GameTags.h"

namespace Nyota
{
// clang-format off

// Input
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "Nyota.InputTag.Move", "Move input.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "Nyota.InputTag.Look.Mouse", "Look (mouse) input.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "Nyota.InputTag.Look.Stick", "Look (stick) input.");

// Ability
UE_DEFINE_GAMEPLAY_TAG(Ability_LightAttack, "Nyota.Ability.Attack.Light");
UE_DEFINE_GAMEPLAY_TAG(Ability_LeftLightAttack, "Nyota.Ability.Attack.LeftLight");
UE_DEFINE_GAMEPLAY_TAG(Ability_RightLightAttack, "Nyota.Ability.Attack.RightLight");
UE_DEFINE_GAMEPLAY_TAG(Ability_ActivateOnGive, "Nyota.Ability.ActivateOnGive");
UE_DEFINE_GAMEPLAY_TAG(Ability_WaterBall, "Nyota.Ability.WaterBall");
UE_DEFINE_GAMEPLAY_TAG(Ability_WaterBubble, "Nyota.Ability.WaterBubble");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab, "Nyota.Ability.Grab");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_PutDown, "Nyota.Ability.Grab.PutDown");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_Slam, "Nyota.Ability.Grab.Slam");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_VortexGrip, "Nyota.Ability.Grab.VortexGrip");
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_PressureBlast, "Nyota.Ability.Grab.PressureBlast");

UE_DEFINE_GAMEPLAY_TAG(Ability_Charge, "Nyota.Ability.Charge");
UE_DEFINE_GAMEPLAY_TAG(Ability_Charge_LightAttack, "Nyota.Ability.Charge.LightAttack");

UE_DEFINE_GAMEPLAY_TAG(Ability_MudShield, "Nyota.Ability.MudShield");
UE_DEFINE_GAMEPLAY_TAG(Ability_Slam, "Nyota.Ability.Slam");

UE_DEFINE_GAMEPLAY_TAG(UI_OpenMap, "Nyota.UI.OpenMap");

UE_DEFINE_GAMEPLAY_TAG(Ability_State, "Nyota.Ability.State")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing, "Nyota.Ability.State.Grabbing")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_PutDownWindow, "Nyota.Ability.State.Grabbing.PutDownWindow")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_Slam, "Nyota.Ability.State.Grabbing.Slam")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_VortexGrip, "Nyota.Ability.State.Grabbing.VortexGrip")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_PressureBlast, "Nyota.Ability.State.Grabbing.PressureBlast")

UE_DEFINE_GAMEPLAY_TAG(Ability_State_Charging, "Nyota.Ability.State.Charging")
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Charging_Bulldozer, "Nyota.Ability.State.Bulldozer")

UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Blocked_Movement, "Nyota.Status.Blocked.Movement", "屏蔽移动：冻结 MovementMode，解除时恢复。");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Blocked_AI, "Nyota.Status.Blocked.AI", "屏蔽 AI：停掉 AIController Tick，行为树整体冻结。");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Blocked_Ability, "Nyota.Status.Blocked.Ability", "屏蔽技能：技能输入在 ASC 入口被拦截。");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Immune_Control, "Nyota.Status.Immune.Control", "控制免疫：施加控制类状态前检查此 Tag。");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Slowed, "Nyota.Status.Slowed", "减速：移动速度按 SlowSpeedMultiplier 降低，计数归零恢复。");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Shielded, "Nyota.Status.Shielded", "护盾：泥土护盾增益期间持有，供查询当前存在护盾。");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Behavior_SurvivesDeath, "Nyota.Ability.Behavior.SurvivesDeath", "An ability with this type tag should not be canceled due to death.");

UE_DEFINE_GAMEPLAY_TAG(Ability_InputBlocked, "Nyota.Ability.AbilityInputBlocked");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "Nyota.InitState.Spawned", "1: Actor/component has initially spawned and can be extended");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "Nyota.InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "Nyota.InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "Nyota.InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");

// Cooldown
UE_DEFINE_GAMEPLAY_TAG(CooldownDuration, "Nyota.Ability.CooldownDuration");
UE_DEFINE_GAMEPLAY_TAG(Passive_Cooldown, "Nyota.Ability.Passive_Cooldown");
UE_DEFINE_GAMEPLAY_TAG(Q_Cooldown, "Nyota.Ability.Q_Cooldown");
UE_DEFINE_GAMEPLAY_TAG(E_Cooldown, "Nyota.Ability.E_Cooldown");
UE_DEFINE_GAMEPLAY_TAG(R_Cooldown, "Nyota.Ability.R_Cooldown");

// Data
UE_DEFINE_GAMEPLAY_TAG(Data_DamageReduction, "Nyota.Data.DamageReduction");

// Event
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_AttackStart, "Nyota.Event.Ability.AttackStart")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_AttackEnd, "Nyota.Event.Ability.AttackEnd")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_LightAttack, "Nyota.Event.Ability.LightAttack")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_ShootWaterBall, "Nyota.Event.Ability.ShootWaterBall")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_WaterBubbleEnd, "Nyota.Event.Ability.WaterBubbleEnd")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_StartGrabTrace, "Nyota.Event.Ability.StartGrabTrace")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_StopGrabTrace, "Nyota.Event.Ability.StopGrabTrace")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_GrabEnd, "Nyota.Event.Ability.GrabEnd")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_PutDownEnd, "Nyota.Event.Ability.PutDownEnd")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_ApplyDamage, "Nyota.Event.Ability.ApplyDamage")
UE_DEFINE_GAMEPLAY_TAG(Event_Enemy_HitReact, "Nyota.Event.Enemy.HitReact")
UE_DEFINE_GAMEPLAY_TAG(Event_Enemy_BoarLanded, "Nyota.Event.Enemy.BoarLanded")
UE_DEFINE_GAMEPLAY_TAG(Event_Item_PickUp, "Nyota.Event.Item.PickUp")

// clang-format on
} // namespace Nyota
