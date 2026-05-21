@echo off
REM Run anytime — puts "Hitman 3d by jss" back on Desktop + OneDrive Desktop
setlocal
title Restore Hitman 3d shortcut
set "ENGINE_ROOT=%~dp0"
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"
set "LAUNCHER=%ENGINE_ROOT%\LAUNCH_HITMAN3D_STUDIO.bat"

echo.
echo  Restoring shortcuts (Desktop + OneDrive Desktop)...
echo.

powershell -NoProfile -ExecutionPolicy Bypass -File "%ENGINE_ROOT%\scripts\hitman3d\repair_shortcut.ps1" -LauncherPath "%LAUNCHER%" -EngineRoot "%ENGINE_ROOT%"

echo.
echo  Opening your Desktop folder...
explorer "%USERPROFILE%\Desktop"
if exist "%USERPROFILE%\OneDrive\Desktop" explorer "%USERPROFILE%\OneDrive\Desktop"
echo.
pause
