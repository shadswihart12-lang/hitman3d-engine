@echo off
REM Double-click from Desktop shortcut — opens Project Manager for Hitman3DStudio
setlocal
set "ENGINE_ROOT=%~dp0"
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"
set "PROJECT_PATH=%ENGINE_ROOT%..\projects\Hitman3DStudio"
for %%I in ("%PROJECT_PATH%") do set "PROJECT_PATH=%%~fI"

title Hitman 3d by jss

set "PM=%ENGINE_ROOT%build\windows\bin\profile\Editor\O3DEProjectManager.exe"
if exist "%PM%" goto launch_pm
set "PM=%ENGINE_ROOT%build\bin\profile\Editor\O3DEProjectManager.exe"
if exist "%PM%" goto launch_pm

REM Fall back: common O3DE installer locations
for %%P in (
    "%ProgramFiles%\O3DE\*\Editor\O3DEProjectManager.exe"
    "%ProgramFiles%\Amazon\O3DE\*\Editor\O3DEProjectManager.exe"
    "%LOCALAPPDATA%\O3DE\*\Editor\O3DEProjectManager.exe"
) do (
    if exist %%P (
        set "PM=%%P"
        goto launch_pm
    )
)

echo.
echo  Project Manager not found yet.
echo.
echo  Engine:  %ENGINE_ROOT%
echo  Project: %PROJECT_PATH%
echo.
echo  Either:
echo    1. Build the engine in Project Manager / Visual Studio, OR
echo    2. Install O3DE from https://o3de.org/download/
echo       then open project Hitman3DStudio at the path above.
echo.
start "" "https://o3de.org/download/"
pause
exit /b 1

:launch_pm
start "" "%PM%"
exit /b 0
