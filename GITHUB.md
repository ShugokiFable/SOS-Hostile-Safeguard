# GitHub

| | |
|--|--|
| **Repo** | https://github.com/ShugokiFable/SOS-Hostile-Safeguard |
| **Clone** | `git clone https://github.com/ShugokiFable/SOS-Hostile-Safeguard.git` |
| **Account** | ShugokiFable |
| **Default branch** | `main` |
| **This folder** | Canonical project + publish home |

## What to push

- `README.md`, `LICENSE`, `CHANGELOG.md`, `docs/`, `GITHUB.md`
- `release/<version>/` ship tree (functional files only)
- `source-publish/SimpleOffenceSuppression/` fork sources (**without** `extern/CommonLibSSE`, **without** `build-ae`)

## Exclude

- `source/` full local tree with CommonLib + build caches
- `dist/`, `*.zip`, `SOS Hostile Safeguard 1.0.x/` working copies if duplicated by `release/`
- secrets, game masters, vcpkg installed trees

## Release tag

```powershell
git tag -a v1.0.1 -m "SOS Hostile Safeguard 1.0.1 — Weylin / HostileNPCs"
git push origin v1.0.1
# optional: gh release create v1.0.1 dist\SOS-Hostile-Safeguard-1.0.1-NEXUS.zip
```
