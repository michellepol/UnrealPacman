# Agent Notes: Pacman

Unreal Engine 5.7 project — classic Pacman clone on Linux. Engine root is `/opt/unreal-engine` and the project path contains spaces, so quote paths in shell commands.

## Project structure

- Single game module: `Pacman` (runtime module only; editor target reuses it).
- C++ source: `Source/Pacman/Private/` and `Source/Pacman/Public/`.
- Major domains: `AI/` (ghosts/behavior trees), `Level/` (grid, pathfinding), `Pickup/`, `Utils/`.
- Blueprints and levels live under `Content/`; default map is `GridMap`, default gamemode is `BP_PacmanGameMode`.
- Ghost AI follows the behavior described at https://gameinternals.com/understanding-pac-man-ghost-behavior.

## Build

The preferred way is `ue4` (`ue4cli`) installed in the project virtual environment `.venv` via `uv`. It auto-detects the project in the current directory and points to `/opt/unreal-engine`.

Activate the environment first:

```bash
source ".venv/bin/activate"
```

Then use `ue4` as usual:

```bash
# Fastest local iteration — editor development build
ue4 build

# Generate IDE project files / Makefile
ue4 gen

# Run automation tests
ue4 test --filter Pacman.Tests
```

To set up the environment from scratch (or recreate it):

```bash
uv venv
uv pip install -r requirements.txt
```

If the environment is not activated, you can still invoke it directly:

```bash
".venv/bin/ue4" build
```

If `ue4` is unavailable, use the generated Makefile or call UnrealBuildTool directly. The project path has spaces, so quote it.

```bash
# Fastest local iteration — editor development build
make PacmanEditor

# Game build
make Pacman

# Full clean rebuild
make PacmanEditor ARGS="-clean"
```

Equivalent raw UBT call:

```bash
"/opt/unreal-engine/Engine/Binaries/ThirdParty/DotNet/6.0.302/linux/dotnet" \
  "/opt/unreal-engine/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll" \
  PacmanEditor Linux Development \
  -project="/home/michelle/documents/Unreal Projects/Pacman/Pacman.uproject"
```

The Makefile is auto-generated — do not hand-edit it. Regenerate IDE/project files with `make configure` if needed.

## IDE / intellisense

- VS Code workspace: `Pacman.code-workspace`. It includes both the project and `/opt/unreal-engine`.
- C++ intellisense relies on `compile_commands.json`. Regenerate it with:

```bash
bash generate_compile_commands.sh
```

This runs `Build.sh -mode=GenerateClangDatabase` and copies `compile_commands.json` from the engine root into the repo.

## Tests

Tests are UE Automation Tests using `EditorContext | EngineFilter`. They live next to the code they test, e.g.:

- `Source/Pacman/Private/Utils/MathTest.cpp`
- `Source/Pacman/Private/Utils/MatrixTest.cpp`
- `Source/Pacman/Private/Level/PathFindingTest.cpp`

Preferred way using `ue4cli`:

```bash
ue4 test --filter Pacman.Tests
```

Fallback — build the editor in Test configuration and run the automation commandlet directly:

```bash
make PacmanEditor-Linux-Test

/opt/unreal-engine/Engine/Binaries/Linux/UnrealEditor \
  "/home/michelle/documents/Unreal Projects/Pacman/Pacman.uproject" \
  -ExecCmds="Automation RunTests Pacman.Tests" \
  -testexit="Automation Test Queue Empty" \
  -nullrhi - unattended
```

## Important dependencies

- `Pacman.Build.cs` declares public deps: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`.
- Private deps: `AIModule`, `NavigationSystem`.
- `Pacman.uproject` also references `AIModule`, `UnrealEd`, `Navmesh`, `AutomationTest`, `NavigationSystem` as additional module dependencies.

## Conventions

- Headers are referenced with module-relative paths, e.g. `#include "Utils/Math.h"`.
- PCH is enabled (`UseExplicitOrSharedPCHs`); include `CoreMinimal.h` via `Pacman.h`.
- Build settings version is V6 and include order is `Unreal5_7`.

## Generated artifacts

Do not commit these (they are already gitignored):

- `Binaries/`, `Intermediate/`, `DerivedDataCache/`, `Saved/`, `.cache/`
- `.vscode/compileCommands_*.json` and generated Makefile
- `compile_commands.json` in the repo root
