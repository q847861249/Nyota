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

function BP_Player:OnSkill1Started(ActionValue)
    self:WaterBallAttack()
    self:GrabSlamAttack()
end

return BP_Player
