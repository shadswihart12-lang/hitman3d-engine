@echo off
REM Opens O3DE Project Manager pointed at your Hitman 3d engine (after setup).

set "ENGINE_ROOT=%~dp0..\.."
REM Prefer engine-root launcher if present
if exist "%~dp0..\..\LAUNCH_HITMAN3D_STUDIO.bat" (
    call "%~dp0..\..\LAUNCH_HITMAN3D_STUDIO.bat"
    exit /b %ERRORLEVEL%
)
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"

set "PM=%ENGINE_ROOT%\build\windows\bin\profile\Editor\O3DEProjectManager.exe"
if exist "%PM%" (
    start "" "%PM%"
    exit /b 0
)

set "PM2=%ENGINE_ROOT%\build\bin\profile\Editor\O3DEProjectManager.exe"
if exist "%PM2%" (
    start "" "%PM2%"
    exit /b 0
)

echo Project Manager not found - build the engine first, or install O3DE Editor.
echo Expected: %PM%
pause
