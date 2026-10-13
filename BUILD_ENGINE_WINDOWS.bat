@echo off
REM ═══════════════════════════════════════════════════════════════
REM BUILD_ENGINE_WINDOWS.bat — build the Hitman3D fork on Windows:
REM   configure (windows-default preset) + build ProjectManager
REM   (o3de.exe, Hitman3D-branded). Long build — let it run.
REM Log: %ENGINE%\BUILD_ENGINE_WINDOWS.log
REM ═══════════════════════════════════════════════════════════════
setlocal EnableDelayedExpansion
set ENGINE=C:\JSS\hitman3d-engine
set LOG=%ENGINE%\BUILD_ENGINE_WINDOWS.log
echo === BUILD_ENGINE_WINDOWS %date% %time% === > "%LOG%"

REM ── locate Visual Studio (vsdevcmd for MSVC env) ──
set VSWHERE="C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist %VSWHERE% (
    echo [X] vswhere not found - is Visual Studio 2026 installed with the C++ workload? >> "%LOG%"
    type "%LOG%" & pause & exit /b 1
)
for /f "usebackq tokens=*" %%i in (`%VSWHERE% -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
    echo [X] No VS with C++ tools found. Install 'Desktop development with C++' via VS Installer. >> "%LOG%"
    type "%LOG%" & pause & exit /b 1
)
echo VS at: !VSPATH! >> "%LOG%"
call "!VSPATH!\Common7\Tools\VsDevCmd.bat" -arch=x64 >> "%LOG%" 2>&1

REM ── cmake on PATH after VsDevCmd? ──
where cmake >nul 2>&1
if errorlevel 1 (
    for /f "delims=" %%C in ('dir /b /s "!VSPATH!\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" 2^>nul ^| findstr cmake.exe') do set "CMAKE_EXE=%%C"
) else (
    set CMAKE_EXE=cmake
)
if not defined CMAKE_EXE (
    echo [X] cmake still not found after VS env. >> "%LOG%"
    type "%LOG%" & pause & exit /b 1
)
echo cmake: !CMAKE_EXE! >> "%LOG%"

cd /d "%ENGINE%"

echo [1/2] Configuring windows-default... >> "%LOG%"
"!CMAKE_EXE!" --preset windows-default -DLY_3RDPARTY_PATH=C:\Users\shads\.o3de\3rdParty >> "%LOG%" 2>&1
echo CONFIGURE_EXIT=!errorlevel! >> "%LOG%"
if not exist "build\windows\build_profile.ninja" (
    if not exist "build\windows\CMakeCache.txt" (
        echo [X] configure failed - tail: >> "%LOG%"
        powershell -NoProfile -Command "Get-Content '%LOG%' | Select-String -Pattern 'CMake Error' -Context 0,6 | Select-Object -First 3" >> "%LOG%"
        type "%LOG%" & pause & exit /b 1
    )
)

echo [2/2] Building ProjectManager (o3de.exe) - LONG, let it run... >> "%LOG%"
"!CMAKE_EXE!" --build build\windows --target ProjectManager --config profile -j%NUMBER_OF_PROCESSORS% >> "%LOG%" 2>&1
echo BUILD_EXIT=!errorlevel! >> "%LOG%"

echo. >> "%LOG%"
if exist "build\windows\bin\profile\o3de.exe" (
    echo === SUCCESS: build\windows\bin\profile\o3de.exe === >> "%LOG%"
) else (
    echo === build finished - check BUILD_EXIT above === >> "%LOG%"
)
type "%LOG%"
echo.
echo Engine build complete (see log). Project Manager = build\windows\bin\profile\o3de.exe
pause