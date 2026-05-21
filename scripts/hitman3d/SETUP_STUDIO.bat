@echo off
REM Hitman 3d by jss — one-click studio setup (Windows)
REM Double-click this file after cloning hitman3d-engine.

setlocal EnableDelayedExpansion

set "ENGINE_ROOT=%~dp0..\.."
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"

echo.
echo  ============================================
echo   Hitman 3d by jss - Studio Setup
echo   Jeffrsin Str33t Studios
echo  ============================================
echo.
echo  Engine folder: %ENGINE_ROOT%
echo.

set "PROJECTS_DIR=%ENGINE_ROOT%\..\projects"
set "PROJECT_PATH=%PROJECTS_DIR%\Hitman3DStudio"

if not exist "%ENGINE_ROOT%\scripts\o3de.bat" (
    echo ERROR: Could not find scripts\o3de.bat in the engine folder.
    echo Make sure you cloned the full hitman3d-engine repo.
    pause
    exit /b 1
)

echo [0/5] Setting up Python for O3DE (first time only, may take several minutes)...
if not exist "%ENGINE_ROOT%\python\get_python.bat" (
    echo ERROR: Missing python\get_python.bat
    pause
    exit /b 1
)
call "%ENGINE_ROOT%\python\get_python.bat"
if errorlevel 1 (
    echo.
    echo ERROR: Python setup failed.
    echo - Install CMake: https://cmake.org/download/  OR use Visual Studio with C++ tools
    echo - Then run this script again.
    pause
    exit /b 1
)

echo.
echo [1/5] Registering Hitman 3d engine...
call "%ENGINE_ROOT%\scripts\o3de.bat" register --this-engine
if errorlevel 1 (
    echo ERROR: Engine registration failed.
    pause
    exit /b 1
)

echo.
echo [2/5] Creating studio project at:
echo        %PROJECT_PATH%
if not exist "%PROJECTS_DIR%" mkdir "%PROJECTS_DIR%"

call "%ENGINE_ROOT%\scripts\o3de.bat" create-project -pp "%PROJECT_PATH%" -pn Hitman3DStudio -tn Hitman3DStudio --force
if errorlevel 1 (
    echo ERROR: Project creation failed.
    pause
    exit /b 1
)

echo.
echo [3/5] Registering project and enabling gems...
call "%ENGINE_ROOT%\scripts\o3de.bat" register --project-path "%PROJECT_PATH%" -f
call "%ENGINE_ROOT%\scripts\o3de.bat" enable-gem -gn Hitman3D_Director -pp "%PROJECT_PATH%" -f
call "%ENGINE_ROOT%\scripts\o3de.bat" enable-gem -gn Hitman3D_AssetHub -pp "%PROJECT_PATH%" -f

echo.
echo [4/5] Done!
echo.
echo  NEXT STEPS (easy):
echo  --------------------------------------------
echo  1. Install O3DE prerequisites if you have not yet
echo     https://docs.o3de.org/docs/welcome-guide/setup/
echo.
echo  2. Open the project in O3DE Project Manager, or build:
echo        cd /d "%PROJECT_PATH%"
echo        cmake -B build/windows -S . -G "Visual Studio 17 2022"
echo.
echo  3. Open O3DE Editor - look for the Director panel
echo     Try typing: create a box
echo.
echo  4. Optional Sketchfab: paste your API token in:
echo     %PROJECT_PATH%\Registry\hitman3d_studio.setreg
echo     under ManualBearerToken
echo.
echo  Project folder: %PROJECT_PATH%
echo.
pause
