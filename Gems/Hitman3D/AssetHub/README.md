# Hitman3D_AssetHub

One-click asset import from Sketchfab, Quixel Megascans, OpenGameArt, and (pending Epic approval) Fab.

## Build & enable

This Gem is staged in `C:\Hitman3D\gem_staging\AssetHub\` and copied into `engine/Gems/Hitman3D/AssetHub/` by `scripts/03_rebrand_engine.py` after the fork is cloned.

To enable on a project:

```powershell
cd C:\Hitman3D\engine
python scripts/o3de/o3de.py enable-gem -gn Hitman3D_AssetHub -pp C:\Hitman3D\projects\<YourProject>
```

Then rebuild the project.

## Structure

```
gem.json
CMakeLists.txt
Code/
  CMakeLists.txt
  assethub_files.cmake
  Include/Hitman3D/AssetHub/    ← public headers
    AssetHubBus.h               ← EBus interface
    AssetMetadata.h             ← shared data structs
    IAssetSource.h              ← source adapter contract
  Source/
    AssetHubSystemComponent.*   ← main runtime component
    AssetHubModule.cpp          ← Gem registration
    Sources/                    ← per-marketplace adapters
      SketchfabSource.*           full impl
      MegascansSource.*           stub
      OpenGameArtSource.*         stub
      FabSource.*                 placeholder (pending Epic approval)
    Pipeline/
      AssetImporter.*           ← download → unpack → SceneAPI → sidecar
    Auth/
      CredentialStore.h
      CredentialStore_Windows.cpp  ← Win32 CredMan backend
```

## v1 limitations (known)

- Only Sketchfab actually fetches assets. Other sources return "not implemented" until wired up.
- Only static meshes supported. Materials/skeletals/HDRIs in v2.
- Sketchfab OAuth2 device flow is partially stubbed — for v1 use manual bearer token set via the settings registry key `/Hitman3D/AssetHub/Sketchfab/ManualBearerToken`.
- Editor UI panel (Qt) not yet implemented — for v1 invoke via Python automation or C++ test harness.
- No archive (zip) unpacking yet — works with direct GLB/FBX downloads.

## See also

- Full design doc: `C:\Hitman3D\docs\ASSETHUB_DESIGN.md`
- Fab API application: `C:\Hitman3D\docs\FAB_API_APPLICATION.md`
