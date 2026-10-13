@echo off
REM ═══════════════════════════════════════════════════════════════
REM START_PIT.bat — create + register the PIT PROTOCOL game project
REM (The Pit, Phase A) on this Windows machine, with the gem enabled.
REM House pattern: double-click. Log: %ENGINE%\START_PIT.log
REM ═══════════════════════════════════════════════════════════════
setlocal
set ENGINE=C:\JSS\hitman3d-engine
set PROJECT=C:\JSS\Projects\PitProtocol
set LOG=%ENGINE%\START_PIT.log
echo === START_PIT %date% %time% === > "%LOG%"

if not exist "%ENGINE%\python\python.bat" (
    echo [X] Engine python not found at %ENGINE%\python — run git pull in %ENGINE% first. >> "%LOG%"
    type "%LOG%"
    pause
    exit /b 1
)

echo [1/4] Creating project (if missing)... >> "%LOG%"
if not exist "%PROJECT%\project.json" (
    call "%ENGINE%\python\python.bat" "%ENGINE%\scripts\o3de.py" create-project --project-path "%PROJECT%" >> "%LOG%" 2>&1
    echo CREATE_EXIT=!errorlevel! >> "%LOG%"
) else (
    echo Project exists. >> "%LOG%"
)

echo [2/4] Registering engine... >> "%LOG%"
call "%ENGINE%\python\python.bat" "%ENGINE%\scripts\o3de.py" register --this-engine >> "%LOG%" 2>&1
echo [3/4] Registering project... >> "%LOG%"
call "%ENGINE%\python\python.bat" "%ENGINE%\scripts\o3de.py" register --project-path "%PROJECT%" >> "%LOG%" 2>&1

echo [4/4] Enabling gem Hitman3D_PitProtocol... >> "%LOG%"
call "%ENGINE%\python\python.bat" "%ENGINE%\scripts\o3de.py" enable-gem -gn Hitman3D_PitProtocol -pp "%PROJECT%" >> "%LOG%" 2>&1
echo ENABLE_EXIT=!errorlevel! >> "%LOG%"

echo. >> "%LOG%"
echo === DONE. Project: %PROJECT% === >> "%LOG%"
echo Next: build it with CMake presets (windows) or open in Project Manager. >> "%LOG%"
type "%LOG%"
echo.
echo PIT PROTOCOL project staged at %PROJECT%
pause