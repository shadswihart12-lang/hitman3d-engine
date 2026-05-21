# Getting started — Hitman 3d by jss

You do **not** need to be comfortable with the command line. Use the steps below.

## What you need on your PC (one time)

1. **Clone** this repo (you already have `hitman3d-engine`).
2. **Install O3DE prerequisites** for Windows:  
   https://docs.o3de.org/docs/welcome-guide/setup/setup-requirements/
3. **Build the engine** once (Project Manager or the build guide). This takes a while the first time.

Use branch **`cursor/hitman3d-audit-fixes-5f89`** or merge [PR #3](https://github.com/shadswihart12-lang/hitman3d-engine/pull/3) into your working branch — it has the fixes for Director, AssetHub, and gem registration.

## One-click setup (Windows)

1. In File Explorer, go to your engine folder.
2. Open `scripts\hitman3d\`
3. **Double-click `SETUP_STUDIO.bat`**

That script will:

- Register the Hitman 3d engine on your machine  
- Create a game project at `..\projects\Hitman3DStudio` (next to the engine folder)  
- Turn on **Director** (AI-style commands in the editor) and **AssetHub** (Sketchfab import)

Wait until it says **Done** and shows the project path.

## Open the editor

After the engine is built:

- Double-click `scripts\hitman3d\OPEN_PROJECT_MANAGER.bat`, **or**
- Open **O3DE Project Manager** from the Start menu and select project **Hitman3DStudio**.

Open the project, then launch the **Editor**.

## Try Director (no coding)

In the Editor, find the **Director** panel (Hitman 3d). Type plain English, for example:

- `help`
- `create a box`
- `add a light at 0 10 0`
- `undo`

Director uses keyword matching today (not a cloud AI yet), but it runs real editor actions.

## Sketchfab assets (optional)

1. Get a Sketchfab API token from your Sketchfab account settings.  
2. Open the project file:  
   `projects\Hitman3DStudio\Registry\hitman3d_studio.setreg`  
3. Paste the token between the quotes at `ManualBearerToken`.  
4. Restart the Editor.

## If something goes wrong

| Problem | What to do |
|--------|------------|
| Setup script errors | Make sure Python exists in `engine\python\` (comes with the repo). Run setup again. |
| Project not listed | Run `SETUP_STUDIO.bat` again. |
| No Director panel | Enable gem **Hitman3D_Director** in Project Manager → Configure Gems. |
| AssetHub / download fails | Install [curl](https://curl.se/windows/) and add it to PATH, or use Git Bash. |
| Build errors | See [docs/HITMAN3D_STATUS.md](docs/HITMAN3D_STATUS.md) |

## More detail

- [docs/HITMAN3D_STATUS.md](docs/HITMAN3D_STATUS.md) — technical audit  
- [Gems/Hitman3D/Director/README.md](Gems/Hitman3D/Director/README.md) — Director commands  
- [Gems/Hitman3D/AssetHub/README.md](Gems/Hitman3D/AssetHub/README.md) — AssetHub limits  

Built by **Jeffrsin Str33t Studios**.
