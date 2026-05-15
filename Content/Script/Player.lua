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
    "/Game/Input/InputAction/IA_Skill_1.IA_Skill_1",
    "Started",
    function(self, ActionValue, A, B)
        self:OnSkill1Started(ActionValue)
    end
)
UnLua.EnhancedInput.BindAction(
    BP_Player,
    "/Game/Input/InputAction/IA_Skill_2.IA_Skill_2",
    "Started",
    function(self, ActionValue, A, B)
        self:OnSkill2Started(ActionValue)
    end
)
UnLua.EnhancedInput.BindAction(
    BP_Player,
    "/Game/Input/InputAction/IA_Skill_2.IA_Skill_2",
    "Started",
    function(self, ActionValue, A, B)
        self:OnSkill2Started(ActionValue)
    end
)
UnLua.EnhancedInput.BindAction(
    BP_Player,
    "/Game/Input/InputAction/IA_Skill_2.IA_Skill_2",
    "Completed",
    function(self, ActionValue, A, B)
        self:OnSkill2Completed(ActionValue)
    end
)
UnLua.EnhancedInput.BindAction(
    BP_Player,
    "/Game/Input/InputAction/IA_Skill_2.IA_Skill_2",
    "Canceled",
    function(self, ActionValue, A, B)
        self:OnSkill2Completed(ActionValue)
    end
)
UnLua.EnhancedInput.BindAction(
    BP_Player,
    "/Game/Input/InputAction/IA_Skill_3.IA_Skill_3",
    "Started",
    function(self, ActionValue, A, B)
        self:OnSkill3Started(ActionValue)
    end
)

-- local WaterBallTag = UE.FGameplayTag.RequestGameplayTag("Nyota.Ability.WaterBall", true)
-- print("WaterBallTag: " .. WaterBallTag:ToString())

function BP_Player:OnSkill1Started(ActionValue)
    self:OnSkill_1_Started()
end

function BP_Player:OnSkill2Started(ActionValue)
    self:OnSkill_2_Started()
end

function BP_Player:OnSkill2Completed(ActionValue)
    self:OnSkill_2_Completed()
end

function BP_Player:OnSkill3Started(ActionValue)
    self:OnSkill_3_Started()
end

return BP_Player
