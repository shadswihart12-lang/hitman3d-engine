@echo off
REM ═══════════════════════════════════════════════════════════════
REM START_PIT.bat v2 — bootstrap bundled python, register engine,
REM create + register the PIT PROTOCOL project, enable the gem.
REM House pattern: double-click. Log: START_PIT.log
REM ═══════════════════════════════════════════════════════════════
setlocal EnableDelayedExpansion
set ENGINE=C:\JSS\hitman3d-engine
set PROJECT=C:\JSS\Projects\PitProtocol
set LOG=%ENGINE%\START_PIT.log
echo === START_PIT v2 %date% %time% === > "%LOG%"

if not exist "%ENGINE%\python\python.cmd" (
    echo [X] python.cmd not found - is %ENGINE% a full clone? >> "%LOG%"
    type "%LOG%" & pause & exit /b 1
)

echo [1/5] Bootstrapping bundled python (first run downloads ~40MB)... >> "%LOG%"
call "%ENGINE%\python\get_python.bat" >> "%LOG%" 2>&1
echo PY_BOOTSTRAP_EXIT=!errorlevel! >> "%LOG%"

echo [2/5] Registering engine... >> "%LOG%"
call "%ENGINE%\python\python.cmd" "%ENGINE%\scripts\o3de.py" register --this-engine >> "%LOG%" 2>&1
echo REGISTER_EXIT=!errorlevel! >> "%LOG%"

echo [3/5] Creating project (if missing)... >> "%LOG%"
if not exist "%PROJECT%\project.json" (
    call "%ENGINE%\python\python.cmd" "%ENGINE%\scripts\o3de.py" create-project --project-path "%PROJECT%" >> "%LOG%" 2>&1
    echo CREATE_EXIT=!errorlevel! >> "%LOG%"
) else (
    echo Project exists. >> "%LOG%"
)

echo [4/5] Registering project... >> "%LOG%"
call "%ENGINE%\python\python.cmd" "%ENGINE%\scripts\o3de.py" register --project-path "%PROJECT%" >> "%LOG%" 2>&1

echo [5/5] Enabling gem Hitman3D_PitProtocol... >> "%LOG%"
call "%ENGINE%\python\python.cmd" "%ENGINE%\scripts\o3de.py" enable-gem -gn Hitman3D_PitProtocol -pp "%PROJECT%" >> "%LOG%" 2>&1
echo ENABLE_EXIT=!errorlevel! >> "%LOG%"

echo. >> "%LOG%"
echo === DONE. Project: %PROJECT% === >> "%LOG%"
type "%LOG%"
echo.
echo PIT PROTOCOL project staged at %PROJECT%
pause
