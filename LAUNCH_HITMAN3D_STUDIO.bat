@echo off
REM Hitman 3d by jss — opens O3DE Project Manager (finds official install OR your built engine)
setlocal
set "ENGINE_ROOT=%~dp0"
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"
set "PROJECT_PATH=%ENGINE_ROOT%\..\projects\Hitman3DStudio"
for %%I in ("%PROJECT_PATH%") do set "PROJECT_PATH=%%~fI"

title Hitman 3d by jss

for /f "delims=" %%F in ('powershell -NoProfile -ExecutionPolicy Bypass -File "%ENGINE_ROOT%\scripts\hitman3d\find_o3de_project_manager.ps1" -EngineRoot "%ENGINE_ROOT%" 2^>^&1') do set "PM=%%F"

if not defined PM (
    echo.
    echo  Could not find O3DE Project Manager ^(o3de.exe^).
    echo.
    echo  1. Install from https://o3de.org/download/
    echo  2. Or build your Hitman engine in Visual Studio first.
    echo.
    echo  Your project is ready at:
    echo    %PROJECT_PATH%
    echo.
    start "" "https://o3de.org/download/"
    pause
    exit /b 1
)

echo.
echo  Opening Project Manager:
echo    %PM%
echo.
echo  In Project Manager:
echo    - Open project: Hitman3DStudio
echo    - Engine folder: %ENGINE_ROOT%
echo.
echo  NOTE: Downloaded O3DE shows O3DE branding until you BUILD this fork.
echo        Hitman 3d splash appears in Editor built from hitman3d-engine.
echo.

start "" "%PM%"
exit /b 0
