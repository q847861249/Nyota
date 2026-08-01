#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

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

#endif
