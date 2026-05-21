@echo off
REM ============================================================
REM  Fix O3DE manifest paths + audit desktop shortcuts
REM  Run from: hitman3d-engine\scripts\hitman3d\
REM ============================================================
setlocal EnableDelayedExpansion
title Hitman 3d - Fix Paths and Shortcut

set "ENGINE_ROOT=%~dp0..\.."
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"
set "PROJECT_PATH=%ENGINE_ROOT%\..\projects\Hitman3DStudio"
for %%I in ("%PROJECT_PATH%") do set "PROJECT_PATH=%%~fI"
set "O3DE_BAT=%ENGINE_ROOT%\scripts\o3de.bat"

echo.
echo  ============================================================
echo    Hitman 3d - Path cleanup and shortcut check
echo  ============================================================
echo.
echo  CORRECT engine:  %ENGINE_ROOT%
echo  CORRECT project: %PROJECT_PATH%
echo.
pause

echo.
echo  -------- Step 1: Remove OLD engine registrations --------
echo.

if exist "C:\Hitman3D\engine\engine.json" (
    echo Removing OLD engine: C:\Hitman3D\engine
    call "%O3DE_BAT%" register --engine-path "C:\Hitman3D\engine" --remove
)
if exist "C:\Hitman3D\engine.json" (
    echo Removing OLD engine: C:\Hitman3D
    call "%O3DE_BAT%" register --engine-path "C:\Hitman3D" --remove
)
if exist "C:\Hitman3D-new\engine.json" (
    echo Removing mistaken registration: C:\Hitman3D-new ^(parent folder^)
    call "%O3DE_BAT%" register --engine-path "C:\Hitman3D-new" --remove
)

echo.
echo  -------- Step 2: Register CORRECT engine and project --------
call "%O3DE_BAT%" register --this-engine --force
call "%O3DE_BAT%" register --project-path "%PROJECT_PATH%" --force
call "%O3DE_BAT%" enable-gem -gn Hitman3D_Director -pp "%PROJECT_PATH%" -f
call "%O3DE_BAT%" enable-gem -gn Hitman3D_AssetHub -pp "%PROJECT_PATH%" -f

echo.
echo  -------- Step 3: Where is hitman3d registered now? --------
call "%O3DE_BAT%" get-registration --engine-name hitman3d
echo.

echo  -------- Step 4: Desktop shortcuts (audit) --------
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0audit_shortcuts.ps1" -EngineRoot "%ENGINE_ROOT%"

echo.
echo  -------- Step 5: Wire your desktop shortcut --------
set "LAUNCHER=%ENGINE_ROOT%\LAUNCH_HITMAN3D_STUDIO.bat"
if not exist "%LAUNCHER%" (
    echo ERROR: Missing %LAUNCHER%
    pause
    exit /b 1
)

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0repair_shortcut.ps1" -LauncherPath "%LAUNCHER%" -EngineRoot "%ENGINE_ROOT%"

echo.
echo  Done. Double-click: Hitman 3d by jss  ^(on your desktop^)
echo  It should run: %LAUNCHER%
echo.
pause
