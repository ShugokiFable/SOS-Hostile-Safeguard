# SOS Hostile Safeguard

**Improved fork of [Simple Offence Suppression](https://www.nexusmods.com/skyrimspecialedition/mods/41764)** (powerofthree) that stops temporary *“friendly”* enemies from being protected like civilians.

| | |
|--|--|
| **Version** | 1.0.1 |
| **Game** | Skyrim Special Edition / Anniversary Edition (SKSE + Address Library) |
| **License** | MIT (upstream + this fork) |
| **Nexus-oriented ship** | `release/1.0.1/` (functional files only) |
| **Author (fork)** | [ShugokiFable](https://github.com/ShugokiFable) |
| **Upstream** | [powerof3/SimpleOffenceSuppression](https://github.com/powerof3/SimpleOffenceSuppression) |

---

## The problem

Simple Offence Suppression (and especially **[SOS MCM – Block Friendly Fire](https://www.nexusmods.com/skyrimspecialedition/mods/41774)**) treats any NPC who is **not currently hostile** as a friend when you hit them.

That also catches enemies who are only calm for a moment:

| Example | Why SOS breaks the fantasy |
|---------|----------------------------|
| **Toll / highway bandits** after you pay | Still bandits; surprise attacks do nothing useful |
| **Arvel the Swift** | BanditAllyFaction, flees with the claw — protected as “friend” |
| **Weylin** (Markarth market, attacks Margret) | Base record has **zero factions** — faction-only patches miss him entirely |
| Modded / quest “talk first” foes | Neutral reaction → Friend promotion → no damage / no offence |

Stock SOS is great for accidental hits on guards and shopkeepers. It should **not** protect scripted assassins and paid-off bandits.

---

## What this project is

**Hybrid package:**

1. **Replacement DLL** — `po3_SimpleOffenceSuppression.dll` (same filename as stock SOS; improved **2.3.1** fork).  
   Disable the original Simple Offence Suppression mod folder so only this DLL loads.
2. **Light ESP** — keyword, formlist, and a **patch** to the MCM friendly-fire perk (does not redistribute the full MCM mod).
3. **SPID `_DISTR.ini`** — tags common hostile factions + unique EditorIDs with the exclusion keyword.

| Component | Role |
|-----------|------|
| DLL exclusions | Skip Neutral→Friend promotion for hostiles |
| `HostileFactions` / `HostileNPCs` INI | Data-driven lists (no recompile for new EditorIDs) |
| Keyword + SPID | MCM damage block also skips tagged hostiles |
| Very Aggressive rule | Restores SOS v1.1-style behaviour for high aggression |

---

## Requirements

### Hard

- [SKSE64](https://skse.silverlock.org/)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
- [Simple Offence Suppression MCM – Block Friendly Fire](https://www.nexusmods.com/skyrimspecialedition/mods/41774) (ESP master; perk this package patches)

### Strongly recommended

- [Spell Perk Item Distributor (SPID)](https://www.nexusmods.com/skyrimspecialedition/mods/36869)

### Replaced at runtime

- [Simple Offence Suppression](https://www.nexusmods.com/skyrimspecialedition/mods/41764) **DLL** — do not run stock and this fork side-by-side under the same plugin name.

---

## Install (players)

1. Disable the original **Simple Offence Suppression** mod (po3).
2. Keep **SOS MCM** enabled.
3. Install this mod’s `release/1.0.1` contents (or a Nexus zip built from that tree).
4. Enable **`SOS Hostile Safeguard.esp`** (light / ESPFE) **after** SOS MCM.
5. Launch with SKSE. Optional: check the SKSE log for Hostile Safeguard / exclusion cache lines.

### Package layout

```text
SKSE/Plugins/po3_SimpleOffenceSuppression.dll
SKSE/Plugins/po3_SimpleOffenceSuppression.ini
SOS Hostile Safeguard.esp
SOS Hostile Safeguard_DISTR.ini
```

---

## Configuration

See **[docs/CONFIGURATION.md](docs/CONFIGURATION.md)**.

Quick example — cover another unique assassin:

```ini
[Exclusions]
HostileNPCs = MS01Weylin,MS13Arvel,e3DemoArvel,MyModAssassinEditorID
```

Optional SPID line:

```ini
Keyword = SOS_NoOffenceSuppression|MyModAssassinEditorID
```

---

## Building from source

See **[docs/BUILD.md](docs/BUILD.md)**.

Summary: clone CommonLibSSE `dev` into `source-publish/SimpleOffenceSuppression/extern/CommonLibSSE`, configure with `BUILD_SKYRIMAE=ON`, build Release, deploy the DLL.

---

## Repository layout

```text
README.md                 This file
LICENSE                   MIT (powerofthree + fork)
CHANGELOG.md              Release history
docs/
  BUILD.md                Compile instructions
  CONFIGURATION.md        INI / SPID details
  NEXUS-DESCRIPTION.bbcode
  NEXUS-SHORT-DESCRIPTION.txt
release/1.0.1/            Player-facing ship tree (no docs junk)
source-publish/
  SimpleOffenceSuppression/   Forked plugin sources (no CommonLibSSE binary tree)
```

---

## Compatibility & honesty

- Built for AE / Address Library style loading (1.6.x class runtimes).
- Structural validation (compile, ESP packaging) is done in development; **in-game confirmation** of every encounter is still appreciated — report NPCs that remain undamageable with EditorID if possible.
- True civilians, guards, and genuine allies remain protected by design.
- Other SOS “INI-only tweaks” are unnecessary if you use this package’s INI; never load two `po3_SimpleOffenceSuppression.dll` files.

---

## Credits

| Credit | Contribution |
|--------|----------------|
| **powerofthree** | Original Simple Offence Suppression (design, SKSE plugin, MIT source); SPID |
| **wankingSkeever** (et al.) | SOS MCM – Block Friendly Fire (perk behaviour patched, not rehosted wholesale) |
| **Ryan / CommonLibSSE lineage** | Library used by the upstream plugin family |
| **meh321** | Address Library |
| **ShugokiFable** | Hostile Safeguard fork (exclusions, unique NPC list, MCM perk patch, packaging) |

---

## Permissions

- Upstream SOS allows modification (MIT + Nexus modification permission).
- This repo ships **modified** DLL sources and **new** addon files only.
- Do not sell. Do not convert to other games under upstream conversion rules where they apply to assets.

---

## Links

- Upstream source: https://github.com/powerof3/SimpleOffenceSuppression  
- Upstream Nexus: https://www.nexusmods.com/skyrimspecialedition/mods/41764  
- Issues / PRs: use this repository’s GitHub Issues for the **fork** behaviour (Weylin, exclusions, packaging).
