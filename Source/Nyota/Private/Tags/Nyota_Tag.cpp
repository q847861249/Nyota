#include "Tags/Nyota_Tag.h"

namespace Nyota
{
    namespace Ability
    {
        UE_DEFINE_GAMEPLAY_TAG(Init, "Nyota.Ability.Init");
        UE_DEFINE_GAMEPLAY_TAG(AttackCheck, "Nyota.Ability.AttackCheck");
        UE_DEFINE_GAMEPLAY_TAG(HitReact, "Nyota.Ability.HitReact");
        UE_DEFINE_GAMEPLAY_TAG(Damage, "Nyota.Ability.Damage");
        UE_DEFINE_GAMEPLAY_TAG(Death, "Nyota.Ability.Death");
        UE_DEFINE_GAMEPLAY_TAG(Kill, "Nyota.Ability.Kill");
        namespace Player
        {
            UE_DEFINE_GAMEPLAY_TAG(Primary, "Nyota.Ability.Player.Primary");
            UE_DEFINE_GAMEPLAY_TAG(Q, "Nyota.Ability.Player.Q");
            UE_DEFINE_GAMEPLAY_TAG(E, "Nyota.Ability.Player.E");
            UE_DEFINE_GAMEPLAY_TAG(R, "Nyota.Ability.Player.R");

        }
        namespace Enemy
        {
            UE_DEFINE_GAMEPLAY_TAG(Primary, "Nyota.Ability.Enemy.Primary");
        }
        namespace Item
        {
            UE_DEFINE_GAMEPLAY_TAG(PickUp, "Nyota.Ability.Item.PickUp");
        }
        namespace Data
        {
            UE_DEFINE_GAMEPLAY_TAG(KillReward, "Nyota.Ability.Data.KillReward");
        }
    }
}