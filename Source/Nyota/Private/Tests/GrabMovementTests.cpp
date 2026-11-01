#include "Character/GrabMovementInput.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
/** @brief 验证组合键保持、释放换轴、反向和摇杆输入不会产生斜向输入。 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FNyotaGrabMovementAxisTest, "Nyota.Movement.GrabFourDirections",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FNyotaGrabMovementAxisTest::RunTest(const FString &Parameters)
{
    using NyotaGrabMovement::SelectAxis;
    TestEqual(TEXT("Idle"), SelectAxis(FVector2D::ZeroVector, 0), 0);
    TestEqual(TEXT("W"), SelectAxis(FVector2D(0, 1), 0), 2);
    TestEqual(TEXT("W then A keeps W"), SelectAxis(FVector2D(-1, 1), 2), 2);
    TestEqual(TEXT("Release W switches to A"), SelectAxis(FVector2D(-1, 0), 2), 1);
    TestEqual(TEXT("A then W keeps A"), SelectAxis(FVector2D(-1, 1), 1), 1);
    TestEqual(TEXT("Release A switches to W"), SelectAxis(FVector2D(0, 1), 1), 2);
    TestEqual(TEXT("Same-frame diagonal chooses forward axis"), SelectAxis(FVector2D(1, 1), 0), 2);
    TestEqual(TEXT("Reverse remains on horizontal axis"), SelectAxis(FVector2D(1, 0), 1), 1);
    TestEqual(TEXT("Release all clears priority"), SelectAxis(FVector2D::ZeroVector, 2), 0);
    TestEqual(TEXT("Analog diagonal retains first axis"), SelectAxis(FVector2D(0.8f, 0.3f), 2), 2);
    TestEqual(TEXT("Tiny analog noise is idle"), SelectAxis(FVector2D(1.e-10, 1.e-10), 0), 0);
    return true;
}
#endif
