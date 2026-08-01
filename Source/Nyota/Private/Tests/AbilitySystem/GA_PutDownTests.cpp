#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "AbilitySystem/Abilities/Player/GA_PutDown.h"
#include "GameplayTags/GameTags.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPutDownNativeTagsTest,
    "Nyota.AbilitySystem.PutDown.NativeTags",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FPutDownNativeTagsTest::RunTest(const FString &Parameters)
{
    TestEqual(
        TEXT("Put-down ability tag"),
        Nyota::Ability_Grab_PutDown.GetTag().ToString(),
        FString(TEXT("Nyota.Ability.Grab.PutDown"))
    );
    TestEqual(
        TEXT("Put-down window tag"),
        Nyota::Ability_State_Grabbing_PutDownWindow.GetTag().ToString(),
        FString(TEXT("Nyota.Ability.State.Grabbing.PutDownWindow"))
    );
    TestEqual(
        TEXT("Put-down completion event"),
        Nyota::Event_Ability_PutDownEnd.GetTag().ToString(),
        FString(TEXT("Nyota.Event.Ability.PutDownEnd"))
    );

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPutDownAbilityContractTest,
    "Nyota.AbilitySystem.PutDown.AbilityContract",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FPutDownAbilityContractTest::RunTest(const FString &Parameters)
{
    const UGA_PutDown *AbilityCDO = GetDefault<UGA_PutDown>();

    TestNotNull(TEXT("Put-down ability CDO"), AbilityCDO);
    TestTrue(
        TEXT("Ability identifies itself by exact tag"),
        AbilityCDO->AbilityTags.HasTagExact(Nyota::Ability_Grab_PutDown)
    );
    TestTrue(
        TEXT("Ability requires the put-down window"),
        AbilityCDO->ActivationRequiredTags.HasTagExact(Nyota::Ability_State_Grabbing_PutDownWindow)
    );

    return true;
}

#endif
