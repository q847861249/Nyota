#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Nyota
{
    namespace Ability
    {
        //general
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Init);
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackCheck);
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
        //specific
        namespace Player
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Q);
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(E);
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(R); 
        }
        namespace Enemy
        {

        }

        namespace Item
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(PickUp);  
        }


    }
}