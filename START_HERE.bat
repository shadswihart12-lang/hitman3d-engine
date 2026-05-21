@echo off
REM ============================================================
REM  HITMAN 3D BY JSS — START HERE (double-click this file only)
REM  Jeffrsin Str33t Studios
REM ============================================================

setlocal EnableDelayedExpansion
set "ENGINE_ROOT=%~dp0"
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"

title Hitman 3d - START HERE
color 0A

echo.
echo   ============================================================
echo     HITMAN 3D BY JSS - FIRST-TIME SETUP
echo   ============================================================
echo.
echo   Engine folder:
echo   %ENGINE_ROOT%
echo.
echo   This window will run TWO steps. Do NOT close it early.
echo   Step 1 can take 10-30 minutes the first time.
echo.
pause

echo.
echo   -------- STEP 1 OF 2: Python for O3DE --------
echo.
if not exist "%ENGINE_ROOT%python\get_python.bat" (
    echo ERROR: python\get_python.bat is missing. Re-clone the repo from GitHub.
    pause
    exit /b 1
)
call "%ENGINE_ROOT%python\get_python.bat"
if errorlevel 1 (
    echo.
    echo   STEP 1 FAILED.
    echo.
    echo   You likely need ONE of these installed first:
    echo     - Visual Studio 2022  with  "Desktop development with C++"
    echo     - OR CMake from https://cmake.org/download/
    echo.
    echo   After installing, double-click START_HERE.bat again.
    echo.
    pause
    exit /b 1
)

echo.
echo   -------- STEP 2 OF 2: Studio project --------
echo.
if not exist "%ENGINE_ROOT%scripts\hitman3d\SETUP_STUDIO.bat" (
    echo ERROR: scripts\hitman3d\SETUP_STUDIO.bat is missing.
    echo Run:  git pull origin development
    pause
    exit /b 1
)
call "%ENGINE_ROOT%scripts\hitman3d\SETUP_STUDIO.bat"
exit /b %ERRORLEVEL%
