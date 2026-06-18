# AGENTS.md — Nyota (UE 5.4)

## Build

```powershell
# Build C++ (auto-detects UE 5.4 from registry; falls back to D:\UE\UE_5.4)
.\Script\Build.ps1 -Target Editor             # Development editor
.\Script\Build.ps1 -Target Game -Run          # Development game, then launch
.\Script\Build.ps1 -Target Editor -Clean -Run # Clean + rebuild editor, then launch
```

## TypeScript (PuerTS)

```bash
npm install       # first time only
npm start         # compile TypeScript/ → Content/JavaScript/ (run before launching editor!)
npm run watch     # watch mode
```

- Entrypoint: `TypeScript/Main.ts` — new TS files must be imported here.
- Pattern: Use PuerTS **mixin** (`blueprint.tojs()` + `blueprint.mixin()`), not subclassing.
- Debugging: Attach VSCode to port 8889 (Node.js attach). Enable "Debugger" on `BP_NyotaGameInstance` first. Must compile TS before attaching.

## C++

- Format: Microsoft-based, 120col, 4-space indent, no include sort (`.clang-format`).
- Single active module: `Source/Nyota/` (Public + Private). `Source/Prototype/` is legacy.
- Build deps include: GAS, Enhanced Input, CommonUI, AIModule, Niagara, UMG, PuerTS, UnLua, ModularGameplay.

## Gotchas

- `.vscode/c_cpp_properties.json` references stale "Prototype" paths — regenerate compile commands via UBT if IntelliSense breaks.
- Workspace launch configs hardcode old `E:\unreal_engine5_projects\JhonProject\GameNyota\` binary paths.
- No CI, no tests, no pre-commit hooks configured.
- PuerTS `AutoModeEnable=False` — TS is loaded explicitly, not auto-started.

## Architecture

- GAS (Gameplay Ability System) with Enhanced Input and CommonUI.
- Abilities use tag-based activation currently (see `Docs/InputID-Migration.md` for planned changes).
- UnLua scripts staged as UFS from `Script/` and `UnLua/Content/Script/` (`DefaultGame.ini`).

## Docs

- `Docs/Puerts.md` — TS setup & debugging guide (Chinese).
- `Docs/InputID-Migration.md` — GAS input migration plan.
