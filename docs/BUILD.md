# Building the SKSE plugin (AE)

## Prerequisites

| Tool | Notes |
|------|--------|
| **Visual Studio 2022+** | Desktop development with C++ (MSVC) |
| **CMake** 3.20+ | On `PATH` |
| **vcpkg** | Set `VCPKG_ROOT` to your vcpkg install |
| **Git** | For CommonLibSSE |
| **Address Library** | Runtime dependency (not a compile dependency) |

Target: **Skyrim AE** (`BUILD_SKYRIMAE=ON`), Address Library–based, version-independent metadata.

## One-time setup

From the repository root:

```powershell
cd source-publish\SimpleOffenceSuppression

# CommonLibSSE (powerofthree dev)
git clone --depth 1 --branch dev https://github.com/powerof3/CommonLibSSE.git extern\CommonLibSSE

# Optional: point env explicitly
$env:VCPKG_ROOT = "C:\path\to\vcpkg"
$env:CommonLibSSEPath = (Resolve-Path .\extern\CommonLibSSE).Path
```

vcpkg packages are pulled via `vcpkg.json` (simpleini, spdlog, xbyak, boost-stl-interfaces, rsm-binary-io, …).

## Configure & build

```powershell
# From source-publish\SimpleOffenceSuppression, in a VS x64 dev shell:

cmake -B build-ae -S . `
  -DBUILD_SKYRIMAE=ON `
  -DCOPY_BUILD=OFF `
  -G "Visual Studio 17 2022" -A x64
  # or "Visual Studio 18 2026" if that is your installed generator

cmake --build build-ae --config Release -j 8
```

Output:

```text
build-ae\Release\po3_SimpleOffenceSuppression.dll
```

Copy that DLL into a mod’s `SKSE\Plugins\` (same name as stock SOS — **replaces** the original DLL).

## Source map

| Path | Role |
|------|------|
| `src/Hooks.cpp` / `Hooks.h` | `GetFactionFightReaction` thunk; `ShouldSuppress` gate |
| `src/Exclusions.h` | Faction / NPC base / keyword / aggression cache |
| `src/Settings.h` | INI load including `[Exclusions]` |
| `src/main.cpp` | SKSE plugin entry (AE + SE query path) |

## Design note

The hook only rewrites **Neutral → Friend/Ally** reactions. Exclusions force Neutral (no suppression) so real foes can take offence and damage normally. MCM Block Friendly Fire is patched in the ESP so damage is not multiplied by zero on keyword-tagged hostiles.
