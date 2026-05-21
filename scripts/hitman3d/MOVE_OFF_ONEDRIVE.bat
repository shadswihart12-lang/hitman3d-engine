@echo off
REM Moves Hitman 3d shortcut from OneDrive Desktop to local Desktop only.
setlocal
set "ENGINE_ROOT=%~dp0..\.."
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"
set "LAUNCHER=%ENGINE_ROOT%\LAUNCH_HITMAN3D_STUDIO.bat"

title Move shortcut off OneDrive

echo.
echo  This puts "Hitman 3d by jss" on your LOCAL desktop:
echo    %USERPROFILE%\Desktop
echo.
echo  And removes the copy from OneDrive Desktop if it exists.
echo.
pause

if not exist "%LAUNCHER%" (
    echo ERROR: Run git pull, then ensure LAUNCH_HITMAN3D_STUDIO.bat exists.
    pause
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0repair_shortcut.ps1" -LauncherPath "%LAUNCHER%" -EngineRoot "%ENGINE_ROOT%"

echo.
pause
