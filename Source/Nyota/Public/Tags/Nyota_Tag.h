#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Nyota
{
    namespace Ability
    {
        //general
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Init);
        //specific
        namespace Player
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Q);
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(E);
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(R);  
        }

    }
}