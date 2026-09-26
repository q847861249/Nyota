// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace Nyota
{

// Input
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);

// Ability
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_LightAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_LeftLightAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_RightLightAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateOnGive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_WaterBall);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_WaterBubble);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab_PutDown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab_Slam);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab_VortexGrip);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab_PressureBlast);

// 野猪冲锋（技能一）及其派生普攻
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Charge);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Charge_LightAttack);

// 野猪泥土护盾（技能二）与野猪砸地（技能三）
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_MudShield);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Slam);

//UI
UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_OpenMap);

// Grab State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing_PutDownWindow)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing_Slam)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing_VortexGrip)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing_PressureBlast)

// 冲锋状态：Charging 表示正在冲锋（免疫控制、转向变慢）；Bulldozer 表示处于推土机状态（可派生普攻顶飞敌人）
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Charging)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Charging_Bulldozer)

// 受控屏蔽维度：状态 GE / 技能按维度授予，UNyotaStatusComponent 监听并执行对应冻结反应。
// 新状态（定身/沉默/眩晕…）= 新 GE 资产组合这些维度，无需改角色类
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Blocked_Movement)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Blocked_AI)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Blocked_Ability)

// 控制免疫：拥有该 Tag 的角色不会被 ApplyStatusEffect 施加控制类状态（冲锋中的野猪授予此 Tag）
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Immune_Control)

// 减速维度：状态 GE 授予后由 UNyotaStatusComponent 按 SlowSpeedMultiplier 降低移动速度，归零恢复。
// 与 Blocked.Movement 正交：一个锁模式、一个改速度，可叠加生效
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Slowed)

// 护盾标记：泥土护盾的增益 GE 授予期间持有，供 UI / 其他技能查询"当前有护盾"
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Shielded)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

// Block All Other Ability
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_InputBlocked);

// Init State
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

// Cooldown
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CooldownDuration);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Passive_Cooldown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Q_Cooldown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(E_Cooldown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(R_Cooldown);

// Data：SetByCaller 传参标签，技能运行时把数值写入 GE 资产的 SetByCaller 修饰符（如护盾的减伤比例）
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_DamageReduction);

// Event
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_AttackStart)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_AttackEnd)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_LightAttack)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_ShootWaterBall)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_WaterBubbleEnd)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_StartGrabTrace)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_StopGrabTrace)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_GrabEnd)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_PutDownEnd)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_ApplyDamage)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Enemy_HitReact)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Enemy_BoarLanded)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Item_PickUp)

} // namespace Nyota
