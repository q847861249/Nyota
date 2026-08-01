# Put-Down Gameplay Ability Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a standalone `UGA_PutDown` that releases a grabbed wild boar without launch force when the grab input is pressed again or the five-second holding window expires.

**Architecture:** `UGA_Grab` owns the temporary holding window and exposes it through a loose Gameplay Tag added only after a successful grab. `UGA_PutDown` requires that Tag, performs the release through the existing `ABaseEnemyWildBoar::OnBoarReleased()`, and sends a gameplay event so `UGA_Grab` can end without entering its unchanged throw path. Blueprint classes, AbilitySet entries, and InputTag assignment remain manual configuration.

**Tech Stack:** Unreal Engine 5.4, C++20 via Unreal Build Tool, Gameplay Ability System, Native Gameplay Tags, Unreal Automation Tests.

## Global Constraints

- Do not modify any `.uasset`, including `BPGA_Grab`, `BP_CrabAbilitySet`, or input configuration assets.
- Do not change the body or behavior of `UGA_Grab::OnGrabTimeout()` or `ABaseEnemyWildBoar::OnThrown()`.
- `UGA_PutDown` must call `ABaseEnemyWildBoar::OnBoarReleased()` and must never apply launch velocity.
- The PutDownWindow Tag must be added only after a wild boar is successfully grabbed and removed on every `UGA_Grab` exit path.
- The existing user changes at `Source/Nyota/Public/GA/GA_BaseSkill.h` and `.claude/` are out of scope and must remain untouched.

## File Map

- Create `Source/Nyota/Public/AbilitySystem/Abilities/Player/GA_PutDown.h`: public declaration and GAS activation contract for the new ability.
- Create `Source/Nyota/Private/AbilitySystem/Abilities/Player/GA_PutDown.cpp`: validation, gentle release, player-state reset, event notification, and ability completion.
- Create `Source/Nyota/Private/Tests/AbilitySystem/GA_PutDownTests.cpp`: native Tag and ability-CDO contract tests.
- Modify `Source/Nyota/Public/GameplayTags/GameTags.h`: declare the put-down ability, window-state, and completion-event Tags.
- Modify `Source/Nyota/Private/GameplayTags/GameTags.cpp`: define the exact native Tag strings.
- Modify `Source/Nyota/Public/AbilitySystem/Abilities/Player/GA_Grab.h`: declare timeout activation, completion handling, cleanup override, and window ownership state.
- Modify `Source/Nyota/Private/AbilitySystem/Abilities/Player/GA_Grab.cpp`: listen for completion, add the window after a successful grab, activate put-down at timeout, fail safely, and clean transient state.

---

### Task 1: Native put-down Gameplay Tags

**Files:**
- Create: `Source/Nyota/Private/Tests/AbilitySystem/GA_PutDownTests.cpp`
- Modify: `Source/Nyota/Public/GameplayTags/GameTags.h`
- Modify: `Source/Nyota/Private/GameplayTags/GameTags.cpp`

**Interfaces:**
- Consumes: Unreal `FGameplayTag` and the existing `Nyota` native-tag namespace.
- Produces: `Nyota::Ability_Grab_PutDown`, `Nyota::Ability_State_Grabbing_PutDownWindow`, and `Nyota::Event_Ability_PutDownEnd`.

- [ ] **Step 1: Add a failing native-Tag automation test**

Create the test file with this contract test:

```cpp
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
    TestEqual(TEXT("Put-down ability tag"), Nyota::Ability_Grab_PutDown.ToString(), FString(TEXT("Nyota.Ability.Grab.PutDown")));
    TestEqual(
        TEXT("Put-down window tag"),
        Nyota::Ability_State_Grabbing_PutDownWindow.ToString(),
        FString(TEXT("Nyota.Ability.State.Grabbing.PutDownWindow"))
    );
    TestEqual(
        TEXT("Put-down completion event"),
        Nyota::Event_Ability_PutDownEnd.ToString(),
        FString(TEXT("Nyota.Event.Ability.PutDownEnd"))
    );
    return true;
}

#endif
```

- [ ] **Step 2: Build to verify the test fails**

Run:

```powershell
& 'D:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat' NyotaEditor Win64 Development '-Project=D:\Projects\Unreal\External\Nyota\Nyota.uproject' -WaitMutex -FromMsBuild
```

Expected: compilation fails because the three `Nyota::` Tag symbols are undeclared.

- [ ] **Step 3: Declare and define the native Tags**

Add to the matching sections of `GameTags.h`:

```cpp
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Grab_PutDown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Grabbing_PutDownWindow)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_PutDownEnd)
```

Add to the matching sections of `GameTags.cpp`:

```cpp
UE_DEFINE_GAMEPLAY_TAG(Ability_Grab_PutDown, "Nyota.Ability.Grab.PutDown");
UE_DEFINE_GAMEPLAY_TAG(Ability_State_Grabbing_PutDownWindow, "Nyota.Ability.State.Grabbing.PutDownWindow")
UE_DEFINE_GAMEPLAY_TAG(Event_Ability_PutDownEnd, "Nyota.Event.Ability.PutDownEnd")
```

- [ ] **Step 4: Build and run the focused Tag test**

Run the build command from Step 2, then:

```powershell
& 'D:\Program Files\Epic Games\UE_5.4\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'D:\Projects\Unreal\External\Nyota\Nyota.uproject' -unattended -nop4 -NullRHI '-ExecCmds=Automation RunTests Nyota.AbilitySystem.PutDown.NativeTags; Quit' '-TestExit=Automation Test Queue Empty' '-log=D:\Projects\Unreal\External\Nyota\Saved\Tests\PutDown-NativeTags.log'
```

Expected: build succeeds and the log reports `Nyota.AbilitySystem.PutDown.NativeTags` passed.

- [ ] **Step 5: Commit the native Tags and their test**

```powershell
git add -- Source/Nyota/Public/GameplayTags/GameTags.h Source/Nyota/Private/GameplayTags/GameTags.cpp Source/Nyota/Private/Tests/AbilitySystem/GA_PutDownTests.cpp
git commit -m "功能: 添加轻放技能标签"
```

---

### Task 2: Standalone `UGA_PutDown`

**Files:**
- Create: `Source/Nyota/Public/AbilitySystem/Abilities/Player/GA_PutDown.h`
- Create: `Source/Nyota/Private/AbilitySystem/Abilities/Player/GA_PutDown.cpp`
- Modify: `Source/Nyota/Private/Tests/AbilitySystem/GA_PutDownTests.cpp`

**Interfaces:**
- Consumes: `Nyota::Ability_Grab_PutDown`, `Nyota::Ability_State_Grabbing_PutDownWindow`, `Nyota::Event_Ability_PutDownEnd`, `ABasePlayer::GetGrabbedEnemy()`, `ABasePlayer::ResetGrabbedEnemy()`, and `ABaseEnemyWildBoar::OnBoarReleased()`.
- Produces: `UGA_PutDown(const FObjectInitializer&)` and `UGA_PutDown::ActivateAbility(...)`.

- [ ] **Step 1: Extend the automation test with a failing CDO contract**

Add the class include and test below to `GA_PutDownTests.cpp`:

```cpp
#include "AbilitySystem/Abilities/Player/GA_PutDown.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPutDownAbilityContractTest,
    "Nyota.AbilitySystem.PutDown.AbilityContract",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FPutDownAbilityContractTest::RunTest(const FString &Parameters)
{
    const UGA_PutDown *AbilityCDO = GetDefault<UGA_PutDown>();
    TestNotNull(TEXT("Put-down ability CDO"), AbilityCDO);
    TestTrue(TEXT("Ability identifies itself by exact tag"), AbilityCDO->AbilityTags.HasTagExact(Nyota::Ability_Grab_PutDown));
    TestTrue(
        TEXT("Ability requires the put-down window"),
        AbilityCDO->ActivationRequiredTags.HasTagExact(Nyota::Ability_State_Grabbing_PutDownWindow)
    );
    return true;
}
```

- [ ] **Step 2: Build to verify the test fails**

Run the Task 1 build command.

Expected: compilation fails because `GA_PutDown.h` and `UGA_PutDown` do not exist.

- [ ] **Step 3: Declare `UGA_PutDown`**

Create `GA_PutDown.h` with this public API:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GA_PutDown.generated.h"

UCLASS()
class NYOTA_API UGA_PutDown : public UNyotaGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_PutDown(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData *TriggerEventData
    ) override;
};
```

- [ ] **Step 4: Implement the minimal gentle-release flow**

Create `GA_PutDown.cpp`. The constructor must add the exact ability and required Tags. `ActivateAbility()` must call `Super`, validate the player, cast the grabbed target, call `OnBoarReleased()` only for a valid wild boar, reset `PendingThrownForce` and the grabbed-target reference, send PutDownEnd for every valid player, and end with `bWasCancelled` set when no valid target was released:

```cpp
#include "AbilitySystem/Abilities/Player/GA_PutDown.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/BaseEnemyWildBoar.h"
#include "Character/BasePlayer.h"
#include "GameplayTags/GameTags.h"

UGA_PutDown::UGA_PutDown(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    AbilityTags.AddTag(Nyota::Ability_Grab_PutDown);
    ActivationRequiredTags.AddTag(Nyota::Ability_State_Grabbing_PutDownWindow);
}

void UGA_PutDown::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ABasePlayer *Player = Cast<ABasePlayer>(GetAvatarActorFromActorInfo());
    if (!IsValid(Player))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    ABaseEnemyWildBoar *WildBoar = Cast<ABaseEnemyWildBoar>(Player->GetGrabbedEnemy());
    const bool bReleasedTarget = IsValid(WildBoar);
    if (bReleasedTarget)
    {
        WildBoar->OnBoarReleased();
    }

    Player->PendingThrownForce = -1.f;
    Player->ResetGrabbedEnemy();

    FGameplayEventData Payload;
    Payload.Instigator = Player;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, Nyota::Event_Ability_PutDownEnd, Payload);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, !bReleasedTarget);
}
```

- [ ] **Step 5: Build and run all put-down tests**

Run the Task 1 build command, then run the same editor command with:

```text
Automation RunTests Nyota.AbilitySystem.PutDown; Quit
```

Expected: `NativeTags` and `AbilityContract` both pass.

- [ ] **Step 6: Commit the standalone ability**

```powershell
git add -- Source/Nyota/Public/AbilitySystem/Abilities/Player/GA_PutDown.h Source/Nyota/Private/AbilitySystem/Abilities/Player/GA_PutDown.cpp Source/Nyota/Private/Tests/AbilitySystem/GA_PutDownTests.cpp
git commit -m "功能: 添加独立轻放技能"
```

---

### Task 3: Integrate the five-second put-down window into `UGA_Grab`

**Files:**
- Modify: `Source/Nyota/Public/AbilitySystem/Abilities/Player/GA_Grab.h`
- Modify: `Source/Nyota/Private/AbilitySystem/Abilities/Player/GA_Grab.cpp`

**Interfaces:**
- Consumes: `UAbilitySystemComponent::AddLooseGameplayTag`, `RemoveLooseGameplayTag`, `TryActivateAbilitiesByTag`, and the three native Tags from Task 1.
- Produces: `UGA_Grab::OnPutDownTimeout()`, `UGA_Grab::OnPutDownEnd(FGameplayEventData)`, `UGA_Grab::EndAbility(...)`, and the invariant that no PutDownWindow Tag survives the grab ability.

- [ ] **Step 1: Add the lifecycle declarations**

Add these declarations to `GA_Grab.h`:

```cpp
public:
    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled
    ) override;

    UFUNCTION()
    void OnPutDownTimeout();

    UFUNCTION()
    void OnPutDownEnd(FGameplayEventData EventData);

private:
    void AddPutDownWindowTag();
    void RemovePutDownWindowTag(const FGameplayAbilityActorInfo *ActorInfo);
    void ReleaseGrabbedTargetAsFailSafe();

    bool bOwnsPutDownWindowTag = false;
```

- [ ] **Step 2: Listen for PutDownEnd during activation**

In `UGA_Grab::ActivateAbility()`, create and activate a `UAbilityTask_WaitGameplayEvent` for `Nyota::Event_Ability_PutDownEnd`, binding `EventReceived` to `OnPutDownEnd`. Keep the existing GrabEnd task and all montage delegates unchanged.

- [ ] **Step 3: Open the window only after a successful grab**

In the valid-wild-boar branch of `PerformGrabTrace()` after `OnGrabbed(Player)`:

```cpp
AddPutDownWindowTag();
GetWorld()->GetTimerManager().SetTimer(
    TimerHandle,
    this,
    &ThisClass::OnPutDownTimeout,
    GrabTimerRate,
    false
);
```

Replace only the old timer delegate `&ThisClass::OnGrabTimeout`; do not edit the `OnGrabTimeout()` function body.

- [ ] **Step 4: Implement timeout activation and safe fallback**

Implement `OnPutDownTimeout()` to build a container containing only `Nyota::Ability_Grab_PutDown` and call `TryActivateAbilitiesByTag`. If the ASC is missing or activation returns false, emit an error log and call `ReleaseGrabbedTargetAsFailSafe()`.

Implement the fail-safe to call `OnBoarReleased()` for a valid grabbed wild boar, reset `PendingThrownForce` and `GrabbedEnemy` for a valid player, then end `UGA_Grab` with `bWasCancelled = true`. It must not call `OnGrabTimeout()` or `OnThrown()`.

- [ ] **Step 5: Implement PutDownEnd and idempotent cleanup**

Implement `OnPutDownEnd()` as:

```cpp
void UGA_Grab::OnPutDownEnd(FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
```

Implement `AddPutDownWindowTag()` to add one loose `Nyota::Ability_State_Grabbing_PutDownWindow` Tag through the current ASC and set `bOwnsPutDownWindowTag` only after success.

Implement `RemovePutDownWindowTag()` to remove one Tag only when `bOwnsPutDownWindowTag` is true, then clear the flag. Override `EndAbility()` to clear both `TimerHandle` and `GrabTraceTimerHandle`, call `RemovePutDownWindowTag(ActorInfo)`, and finally call `Super::EndAbility(...)` with the original arguments.

- [ ] **Step 6: Build and run the automation suite**

Run the Task 1 build command and the `Nyota.AbilitySystem.PutDown` automation group.

Expected: compilation succeeds and both contract tests pass.

- [ ] **Step 7: Inspect the focused diff for throw-path preservation**

Run:

```powershell
git diff -- Source/Nyota/Private/AbilitySystem/Abilities/Player/GA_Grab.cpp Source/Nyota/Public/AbilitySystem/Abilities/Player/GA_Grab.h Source/Nyota/Private/Character/BaseEnemyWildBoar.cpp
```

Expected: `UGA_Grab::OnGrabTimeout()` has no body changes, `ABaseEnemyWildBoar.cpp` has no changes, and the timer delegate now targets `OnPutDownTimeout()`.

- [ ] **Step 8: Commit the grab lifecycle integration**

```powershell
git add -- Source/Nyota/Public/AbilitySystem/Abilities/Player/GA_Grab.h Source/Nyota/Private/AbilitySystem/Abilities/Player/GA_Grab.cpp
git commit -m "功能: 抓取期间支持主动与超时轻放"
```

---

### Task 4: Final verification and configuration handoff

**Files:**
- Verify only: all files listed in the File Map.
- Do not modify: `Content/**/*.uasset`.

**Interfaces:**
- Consumes: the completed put-down implementation and the user's future Blueprint/AbilitySet configuration.
- Produces: build/test evidence and exact editor configuration instructions.

- [ ] **Step 1: Run a clean focused verification**

Run the editor-target build and `Nyota.AbilitySystem.PutDown` automation group one final time. Inspect `Saved/Tests/PutDown-NativeTags.log` or the group log for zero failed tests.

- [ ] **Step 2: Verify repository scope**

Run:

```powershell
git status --short
git diff --name-only HEAD~3..HEAD
```

Expected implementation scope: the two new GA files, one new test file, `GameTags.h/.cpp`, and `GA_Grab.h/.cpp`. No `.uasset` appears. The pre-existing deleted `GA_BaseSkill.h` and untracked `.claude/` remain unrelated and uncommitted.

- [ ] **Step 3: Provide the manual editor configuration**

Tell the user to create/configure `BPGA_PutDown`, add it to `BP_CrabAbilitySet`, and assign the exact same InputTag used by `BPGA_Grab`. Explain that the C++ CDO already supplies the ability and required Tags, so they should not duplicate or remove those Tags in Blueprint defaults.

- [ ] **Step 4: Manual PIE acceptance check after user configuration**

In PIE, verify:

1. First grab press starts the grab and does not immediately release.
2. A second press before five seconds gently detaches the wild boar with no launch velocity.
3. With no second press, five-second expiry performs the same gentle release.
4. Slam and Pressure Blast still use their existing throw force.
5. After every release or throw, a new grab can start and `Nyota.Ability.State.Grabbing.PutDownWindow` is not left on the ASC.

- [ ] **Step 5: Commit only if verification required a code correction**

If verification exposes a code defect, add only the files changed for that defect, rerun Steps 1 and 2, and commit with a message describing the exact correction. If no correction is needed, create no empty commit.
