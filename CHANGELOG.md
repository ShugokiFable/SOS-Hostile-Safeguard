# Changelog

All notable changes to **SOS Hostile Safeguard** are documented here.

## [1.0.1] — 2026-08-05

### Fixed

- **Weylin (`MS01Weylin`)** was not covered: his base NPC has **zero factions**, so faction-only exclusions never matched. He is the Markarth market assassin who attacks Margret on first entry.
- Same class of unique scripted “false friendlies” (empty faction list, only temporarily non-hostile).

### Added

- INI key **`HostileNPCs`** — comma-separated **NPC base EditorIDs** resolved at data load (DLL).
- Defaults: `MS01Weylin`, `MS13Arvel`, `e3DemoArvel`.
- SPID unique EditorID lines for Weylin and Arvel so **MCM Block Friendly Fire** also stops zeroing damage.
- FormList exclusion path may include **NPC bases**, not only factions/keywords.
- DLL version **2.3.1**.

### Ship

- Clean Nexus layout: DLL, INI, light ESP, SPID `_DISTR.ini` only.

## [1.0.0] — 2026-08-05

### Added

- Fork of Simple Offence Suppression **2.2.1** → DLL **2.3.0 Hostile Safeguard**.
- Skip friend-style offence promotion for:
  - actors in configured **hostile factions** (INI EditorIDs + optional FormList),
  - actors with keyword **`SOS_NoOffenceSuppression`**,
  - **Very Aggressive / Frenzied** (restored v1.1-style behaviour).
- Light ESP: keyword + `SOS_HostileFactionList` + override of SOS MCM friendly-fire perk (no damage ×0 on keyworded hostiles).
- SPID distribution of the keyword to common criminal/enemy factions.
- Configurable `po3_SimpleOffenceSuppression.ini` `[Exclusions]` section.
