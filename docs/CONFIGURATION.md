# Configuration

File: `Data/SKSE/Plugins/po3_SimpleOffenceSuppression.ini`

## Stock SOS keys

Same as powerofthree’s plugin:

- `SetAsAlly`, `OnlyCombat`, `IgnoreFriendlyFire`, `ModifyGameSettings`
- `iFriendHitNonCombatAllowed`, `iFriendHitCombatAllowed`, `iAllyHitNonCombatAllowed`, `iAllyHitCombatAllowed`

## Hostile Safeguard — `[Exclusions]`

| Key | Default | Meaning |
|-----|---------|---------|
| `ExcludeVeryAggressive` | `true` | Skip suppression if Aggression ≥ Very Aggressive |
| `UseKeywordExclusion` | `true` | Skip if actor has the exclusion keyword |
| `ExclusionKeyword` | `SOS_NoOffenceSuppression` | Keyword EditorID (from the ESP / SPID) |
| `ExclusionFormList` | `SOS_HostileFactionList` | Optional FormList of factions, keywords, and/or NPC bases |
| `HostileFactions` | (long CSV) | Faction **EditorIDs** never suppressed |
| `HostileNPCs` | `MS01Weylin,MS13Arvel,e3DemoArvel` | NPC **base** EditorIDs never suppressed |

### When to use which list

| Situation | Use |
|-----------|-----|
| Bandits / forsworn camps / undead packs | `HostileFactions` or SPID faction lines |
| Unique actor with **empty** faction list (Weylin) | `HostileNPCs` + SPID EditorID line |
| Modded assassin you identify by EditorID | Append to `HostileNPCs` |
| Shareable extension without editing INI | Inject into FormList `SOS_HostileFactionList` |

Restart the game after INI changes (settings load at plugin load / data load).

## SPID

`SOS Hostile Safeguard_DISTR.ini` is loaded by **Spell Perk Item Distributor**.  
It tags factions and uniques with `SOS_NoOffenceSuppression` so:

1. The DLL exclusion keyword path matches.
2. The MCM perk override does not zero damage on those targets.

Without SPID, DLL faction/NPC EditorID lists still work; MCM damage blocking may still apply to untagged uniques if they only exist on the perk path — uniques listed in `HostileNPCs` are still covered by the **DLL** for offence reaction.
