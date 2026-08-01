# Put-Down Gameplay Ability Design

## Goal

Add a standalone `UGA_PutDown` that gently releases a grabbed wild boar. The player can activate it by pressing the grab input again during a five-second window, and `UGA_Grab` activates the same ability automatically when that window expires.

The existing throw path in `UGA_Grab::OnGrabTimeout()` remains unchanged for Slam, Pressure Blast, and other flows that intentionally throw the target.

## Scope

The implementation adds C++ Gameplay Ability and Gameplay Tag support only. Blueprint Ability assets, the crab AbilitySet, and InputTag assignments remain user-configured and are not modified.

## Gameplay Tags

Add these native tags:

- `Nyota.Ability.Grab.PutDown`: identifies `UGA_PutDown` for programmatic activation.
- `Nyota.Ability.State.Grabbing.PutDownWindow`: permits `UGA_PutDown` activation only after a target has been grabbed successfully and before the five-second window closes.
- `Nyota.Event.Ability.PutDownEnd`: notifies `UGA_Grab` that the target was released and its holding lifecycle can end without invoking the throw path.

## Components

### `UGA_PutDown`

`UGA_PutDown` derives from `UNyotaGameplayAbility`. Its constructor adds `Nyota.Ability.Grab.PutDown` to `AbilityTags` and `Nyota.Ability.State.Grabbing.PutDownWindow` to `ActivationRequiredTags`.

On activation, it:

1. Resolves the avatar as `ABasePlayer`.
2. Resolves the player's grabbed target as `ABaseEnemyWildBoar`.
3. Calls the existing `ABaseEnemyWildBoar::OnBoarReleased()`, which detaches the target, restores collision, Walking movement, and AI, and applies no launch force.
4. Resets `ABasePlayer::GrabbedEnemy` and `PendingThrownForce`.
5. Sends `Nyota.Event.Ability.PutDownEnd` to the player.
6. Ends successfully.

If the player is valid but the grabbed target is not, the ability resets the stale player reference, sends PutDownEnd so `UGA_Grab` can clean up, and ends as cancelled. If the player itself is invalid, it ends as cancelled without dereferencing it. It does not call `OnThrown()` under any condition.

### `UGA_Grab`

When the valid-wild-boar branch of `PerformGrabTrace()` successfully grabs a target, `UGA_Grab` adds the PutDownWindow loose Gameplay Tag to the player's ASC and starts its existing five-second hold timer.

The timer callback no longer invokes `OnGrabTimeout()`. It asks the ASC to activate the ability carrying `Nyota.Ability.Grab.PutDown`. Because the PutDownWindow Tag is still present at that point, normal GAS activation checks allow `UGA_PutDown` to run.

`UGA_Grab` also waits for `Nyota.Event.Ability.PutDownEnd`. On receipt it clears the hold timer, removes the PutDownWindow Tag, and ends without calling the throw logic.

`UGA_Grab::EndAbility()` performs idempotent cleanup of the hold timer, grab-trace timer, and PutDownWindow Tag on every exit path. The Tag is removed only if this ability previously added it, preventing an unmatched removal.

`UGA_Grab::OnGrabTimeout()` and its calls to `OnThrown()` remain unchanged. Existing `GrabEnd`, montage interruption, and montage cancellation paths continue to use that throw behavior unless separately changed in the future.

## Input and Data Asset Configuration

The user will:

1. Create a Blueprint subclass of `UGA_PutDown` if Blueprint configuration is desired.
2. Grant it through the crab AbilitySet.
3. Assign it the same InputTag as `GA_Grab`.

On the first button press, `UGA_PutDown` fails its required-Tag check because PutDownWindow is added only after a target is actually grabbed. During the next five seconds, another press activates `UGA_PutDown` normally through the existing input pipeline.

## State Flow

1. First input tries both abilities; only `UGA_Grab` can activate.
2. A successful grab attaches the wild boar, adds PutDownWindow, and starts the five-second timer.
3. The flow then branches:
   - A second input during the window activates `UGA_PutDown`.
   - Timer expiry programmatically activates the same `UGA_PutDown`.
4. `UGA_PutDown` gently releases the target and sends PutDownEnd.
5. `UGA_Grab` receives PutDownEnd, removes transient state, and ends.

Existing sub-abilities can still send GrabEnd and set `PendingThrownForce`; that path continues through the unchanged throw implementation.

## Failure Handling

- Missing player or grabbed target: `UGA_PutDown` cancels safely.
- Missing ASC: no Tag mutation or programmatic activation is attempted.
- `UGA_PutDown` is not granted or activation is blocked at timeout: `UGA_Grab` logs an error, calls the existing `OnBoarReleased()` as a fail-safe, resets the player's grabbed-target state, and ends without invoking `OnThrown()`. This prevents a configuration error from leaving the target permanently attached. In the correctly configured flow, both manual and timeout release run through `UGA_PutDown`.
- Repeated cleanup calls are safe because timer clearing and Tag removal are idempotent.

## Verification

- Build the Nyota editor target to validate Unreal Header Tool and C++ compilation.
- Confirm a first grab input cannot activate `UGA_PutDown` before a target is grabbed.
- Confirm a second grab input within five seconds calls `OnBoarReleased()` and applies no launch velocity.
- Confirm five-second expiry invokes the same put-down path.
- Confirm PutDownWindow is removed after manual put-down, timeout put-down, cancellation, and normal ability end.
- Confirm Slam and Pressure Blast still reach the unchanged `OnGrabTimeout()`/`OnThrown()` path with their configured force.
- Confirm Blueprint and AbilitySet assets remain unmodified by the code change.
