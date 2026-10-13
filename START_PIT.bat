@echo off
REM ═══════════════════════════════════════════════════════════════
REM START_PIT.bat v3 — cmake autodetect (VS-bundled or PATH),
REM bootstrap python, register, create + register project, enable gem.
REM ═══════════════════════════════════════════════════════════════
setlocal EnableDelayedExpansion
set ENGINE=C:\JSS\hitman3d-engine
set PROJECT=C:\JSS\Projects\PitProtocol
set LOG=%ENGINE%\START_PIT.log
echo === START_PIT v3 %date% %time% === > "%LOG%"

REM ── locate cmake ──
set CMAKE_EXE=
where cmake >nul 2>&1 && set CMAKE_EXE=cmake
if not defined CMAKE_EXE (
    for %%V in (2026 2022) do for %%E in (Community Professional Enterprise) do (
        if not defined CMAKE_EXE (
            if exist "C:\Program Files\Microsoft Visual Studio\%%V\%%E\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" (
                set "CMAKE_EXE=C:\Program Files\Microsoft Visual Studio\%%V\%%E\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
            )
        )
    )
)
if not defined CMAKE_EXE (
    echo [X] CMake not found. Run: winget install --id Kitware.CMake -e   then rerun. >> "%LOG%"
    type "%LOG%" & pause & exit /b 1
)
echo Found cmake: !CMAKE_EXE! >> "%LOG%"
set "LY_CMAKE_PATH=!CMAKE_EXE!"
set "PATH=!PATH!;C:\Program Files\Microsoft Visual Studio\2026\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;C:\Program Files\CMake\bin"

echo [0/5] Clearing python env poison + partial venv... >> "%LOG%"
set PYTHONHOME=
set PYTHONPATH=
echo PYTHONHOME/PYTHONPATH cleared. >> "%LOG%"
if exist "C:\Users\shads\.o3de\Python\venv\722b02a4" rmdir /s /q "C:\Users\shads\.o3de\Python\venv\722b02a4"
echo [1/5] Bootstrapping bundled python... >> "%LOG%"
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
