# Hitman 3d by jss — Engine Audit (May 2026)

This document summarizes the state of the **hitman3d-engine** fork and known gaps for the AI automated game studio vision.

## What works today

| Area | Status |
|------|--------|
| O3DE fork + branding | Splash, window titles, `engine.json` / `BRANDING.json` metadata |
| **Director** gem | Keyword NL → engine actions (entities, lights, levels, Lua stubs). Editor Qt panel on `development` after merge. |
| **AssetHub** gem (partial) | Sketchfab search/download path; curl-based HTTP client (replaces removed O3DE HttpRequestor) |

## Critical issues found (and fixes in this branch)

1. **HttpRequestor removed upstream** — O3DE dropped `Gems/HttpRequestor` and `AWSNativeSDKInit` (#18571 / #18635). AssetHub still depended on it → **build failure**. Fixed by `AssetHubHttpClient` (curl subprocess) inside the gem.

2. **Hitman3D gems not registered** — `Gems/Hitman3D/AssetHub` and `Gems/Hitman3D/Director` were absent from `engine.json` `external_subdirectories` → gems invisible to `o3de.py`/Project Manager. Fixed in `engine.json`.

3. **Director not on `development`** — AI “brain” lived only on `cursor/director-gem-5fa2`. Merged into audit branch.

4. **AssetHub editor CMake referenced missing files** — `assethub_editor_files.cmake` listed `AssetHubMainWindow`, `AssetHubEditorModule`, etc. that do not exist. Editor targets disabled until UI is implemented.

5. **CredentialStore on Linux** — Previously returned `false` for all ops. Non-Windows path now uses settings registry under `/Hitman3D/AssetHub/Credentials/`.

6. **Branch `hitman3d/dev` is stale** — Points at upstream `de10c88038` without your branding/AssetHub commits (`1b33b09038`). Use `development` or update `hitman3d/dev`.

## Still incomplete (by design / TODO)

- **AssetHub marketplace sources**: Megascans, OpenGameArt, Fab = stubs.
- **AssetHub editor UI**: No Qt panel yet (README acknowledged; CMake was ahead of code).
- **Zip unpack**: `AssetImporter::Unpack` passes through archives unchanged.
- **Director LLM**: Keyword parser only — no hosted model integration.
- **Templates / sample project**: No Hitman3d project template with gems pre-enabled.
- **README vs reality**: Says `Code/` must not be modified, but branding edits live under `Code/Editor/` (acceptable for fork, doc should be clarified).

## Enabling gems on a project

```bash
python scripts/o3de/o3de.py register --engine-path /path/to/hitman3d-engine
python scripts/o3de/o3de.py enable-gem -gn Hitman3D_Director -pp /path/to/your/project
python scripts/o3de/o3de.py enable-gem -gn Hitman3D_AssetHub -pp /path/to/your/project
```

Sketchfab (v1): set `/Hitman3D/AssetHub/Sketchfab/ManualBearerToken` in user settings until OAuth device flow is finished.

## Recommended next steps

1. Add a **Hitman3D** project template with Director + AssetHub enabled.
2. Wire Director ↔ AssetHub (“find a tree on Sketchfab and place it”).
3. Replace curl subprocess with **libcurl** or in-engine async HTTP for production.
4. Implement AssetHub editor panel or Python automation entry points.
5. Sync `hitman3d/dev` to `development` or retire the stale branch name.
