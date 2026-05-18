--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local BP_Player = UnLua.Class()

-- function BP_Player:Initialize(Initializer)
-- end

-- function BP_Player:UserConstructionScript()
-- end

-- function BP_Player:ReceiveBeginPlay()
-- end

-- function BP_Player:ReceiveEndPlay()
-- end

-- function BP_Player:ReceiveTick(DeltaSeconds)
-- end

-- function BP_Player:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function BP_Player:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function BP_Player:ReceiveActorEndOverlap(OtherActor)
-- end

-- 绑定 Input Action 到 Lua 函数
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_LightAttack.IA_LightAttack",
        "Started",
        function(self, ActionValue, A, B)
            self:OnLightAttack_Started()
        end
)
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_LeftLightAttack.IA_LeftLightAttack",
        "Started",
        function(self, ActionValue, A, B)
            self:OnLeftLightAttack_Started()
        end
)
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_RightLightAttack.IA_RightLightAttack",
        "Started",
        function(self, ActionValue, A, B)
            self:OnRightLightAttack_Started()
        end
)
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_Skill_1.IA_Skill_1",
        "Started",
        function(self, ActionValue, A, B)
            self:OnSkill_1_Started()
        end
)
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_Skill_2.IA_Skill_2",
        "Started",
        function(self, ActionValue, A, B)
            self:OnSkill_2_Started()
        end
)
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_Skill_2.IA_Skill_2",
        "Completed",
        function(self, ActionValue, A, B)

            self:OnSkill_2_Completed()
        end
)
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_Skill_2.IA_Skill_2",
        "Canceled",
        function(self, ActionValue, A, B)
            self:OnSkill_2_Completed()
        end
)
UnLua.EnhancedInput.BindAction(
        BP_Player,
        "/Game/Input/InputAction/IA_Skill_3.IA_Skill_3",
        "Started",
        function(self, ActionValue, A, B)
            self:OnSkill_3_Started(ActionValue)
        end
)

-- local WaterBallTag = UE.FGameplayTag.RequestGameplayTag("Nyota.Ability.WaterBall", true)
-- print("WaterBallTag: " .. WaterBallTag:ToString())

return BP_Player
