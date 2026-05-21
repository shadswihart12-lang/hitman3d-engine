@echo off
REM Run this anytime your desktop shortcut is missing.
setlocal
set "ENGINE_ROOT=%~dp0"
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"
set "LAUNCHER=%ENGINE_ROOT%\LAUNCH_HITMAN3D_STUDIO.bat"

title Create Hitman 3d shortcut

if not exist "%LAUNCHER%" (
    echo Missing: %LAUNCHER%
    pause
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%ENGINE_ROOT%\scripts\hitman3d\repair_shortcut.ps1" -LauncherPath "%LAUNCHER%" -EngineRoot "%ENGINE_ROOT%"

echo.
echo  Look on your Desktop for: Hitman 3d by jss
echo  If you still do not see it, open File Explorer and go to:
echo    %USERPROFILE%\Desktop
echo.
explorer "%USERPROFILE%\Desktop"
pause
