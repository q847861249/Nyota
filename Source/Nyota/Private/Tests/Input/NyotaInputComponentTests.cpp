#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "InputAction.h"
#include "Input/NyotaInputComponent.h"

namespace
{
class FAbilityInputReceiver
{
public:
    void OnPressed(const FGameplayTag &InputTag)
    {
    }

    void OnReleased(const FGameplayTag &InputTag)
    {
    }
};
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FAbilityInputUsesPressEdgeTest,
    "Nyota.Input.AbilityActions.UsePressEdge",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FAbilityInputUsesPressEdgeTest::RunTest(const FString &Parameters)
{
    UNyotaInputComponent *InputComponent = NewObject<UNyotaInputComponent>();
    UNyotaInputConfig *InputConfig = NewObject<UNyotaInputConfig>();
    UInputAction *InputAction = NewObject<UInputAction>();

    FNyotaInputAction AbilityInputAction;
    AbilityInputAction.InputAction = InputAction;
    AbilityInputAction.InputTag = FGameplayTag::RequestGameplayTag(TEXT("Nyota.InputTag.Skill.1"));
    InputConfig->AbilityInputActions.Add(AbilityInputAction);

    FAbilityInputReceiver Receiver;
    TArray<uint32> BindHandles;
    InputComponent->BindAbilityActions(
        InputConfig,
        &Receiver,
        &FAbilityInputReceiver::OnPressed,
        &FAbilityInputReceiver::OnReleased,
        BindHandles
    );

    const TArray<TUniquePtr<FEnhancedInputActionEventBinding>> &Bindings = InputComponent->GetActionEventBindings();
    if (!TestEqual(TEXT("One press and one release binding"), Bindings.Num(), 2))
    {
        return false;
    }

    TestEqual(TEXT("Ability press fires once on the input edge"), Bindings[0]->GetTriggerEvent(), ETriggerEvent::Started);
    TestEqual(TEXT("Ability release fires when input completes"), Bindings[1]->GetTriggerEvent(), ETriggerEvent::Completed);

    return true;
}

#endif
