# Hitman 3d — simple Windows setup (read this if scripts fail)

You are **not** bad at computers. Open 3D Engine is used by studios with IT teams. This guide is the honest minimum.

## What you are trying to do

1. **Prepare tools** (Python, Visual Studio, CMake) — one time  
2. **Register your engine + create project** — `START_HERE.bat`  
3. **Build the engine** — one time, can take **1–3 hours**  
4. **Open Editor** — where Director lives  

Cursor / cloud agents **cannot click on your PC**. They can fix the repo and scripts; **you** still run the installer steps below once.

---

## Before double-clicking anything — install these (one time)

### 1. Visual Studio 2022 (Community is free)

Download: https://visualstudio.microsoft.com/downloads/

During install, check:

- **Desktop development with C++**

### 2. Git (you already have this if clone worked)

### 3. Optional but helpful: CMake

https://cmake.org/download/ — “Add to PATH” if the installer asks.

---

## Desktop shortcut (not OneDrive)

Your engine lives at `C:\Hitman3D-new\` — **outside** OneDrive (good).

To move the shortcut off OneDrive Desktop, double-click:

`scripts\hitman3d\MOVE_OFF_ONEDRIVE.bat`

That puts **Hitman 3d by jss** on `C:\Users\<you>\Desktop` and removes the OneDrive copy.

---

## The only file to double-click (after VS is installed)

In File Explorer open your engine folder:

`C:\Hitman3D-new\hitman3d-engine`

Double-click:

### **`START_HERE.bat`** (in the main folder, not inside scripts)

Wait until Step 2 says **Done**.

Your game project will be:

`C:\Hitman3D-new\projects\Hitman3DStudio`

---

## After START_HERE succeeds — you still must BUILD O3DE

The scripts do **not** build the whole engine automatically. That is normal.

**Easiest path for beginners:**

1. Install the official **O3DE** from https://o3de.org/download/ (if you have not).  
2. Open **O3DE Project Manager**.  
3. Add engine manually pointing to `C:\Hitman3D-new\hitman3d-engine` if needed.  
4. Open project **Hitman3DStudio** and click **Build Project** / **Build Engine**.

First build = long wait. Go get coffee.

When build finishes → **Open Editor** → find **Director** panel → type `help`.

---

## What went wrong for you so far

| Problem | Meaning |
|--------|---------|
| Python venv missing | Step 1 (`get_python.bat`) did not finish — install Visual Studio first |
| Only see “O3DE” | Stock O3DE app vs your fork — use project **Hitman3DStudio** and engine path `hitman3d-engine` |
| Window closes fast | Press Enter only after reading **Done** or **ERROR** |

---

## Get human-style help from Cursor

In Cursor chat, paste **a photo or the full text** from the black window after `START_HERE.bat` or `get_python.bat`. Say:

> “This is the error from Hitman3d setup on Windows”

Do **not** paste only “it failed” — the last 10 lines matter.

---

## Still stuck?

Reply with:

1. Did you install **Visual Studio 2022** with C++? (yes/no)  
2. What happens when you double-click **`START_HERE.bat`**? (copy last 15 lines)  
3. Did you install official **O3DE** from o3de.org? (yes/no)  

We can narrow it to **one** fix instead of ten steps.

Jeffrsin Str33t Studios — Hitman 3d by jss
