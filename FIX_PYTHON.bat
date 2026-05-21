@echo off
REM ============================================================
REM  Fix O3DE Python (run this if you see "Missing Python venv")
REM ============================================================
setlocal EnableDelayedExpansion
title Hitman 3d - FIX PYTHON
set "ENGINE_ROOT=%~dp0"
for %%I in ("%ENGINE_ROOT%") do set "ENGINE_ROOT=%%~fI"

echo.
echo  This will install Python for O3DE into:
echo  %USERPROFILE%\.o3de\Python\venv\
echo.
echo  Requirements:
echo    - CMake on PATH  (install from cmake.org OR use VS Developer Command Prompt)
echo    - Internet connection
echo.
echo  Log file: %ENGINE_ROOT%python_setup_log.txt
echo.
pause

where cmake >nul 2>&1
if errorlevel 1 (
    if exist "%ENGINE_ROOT%cmake\runtime\bin\cmake.exe" (
        set "PATH=%ENGINE_ROOT%cmake\runtime\bin;%PATH%"
        echo Found bundled cmake.
    ) else (
        echo.
        echo  ERROR: CMake not found.
        echo  Fix: Open "Visual Studio Installer" -^> Modify -^> Desktop development with C++
        echo  OR install CMake: https://cmake.org/download/  and check "Add to PATH"
        echo  OR open "x64 Native Tools Command Prompt for VS 2022" and run this bat again from there.
        echo.
        pause
        exit /b 1
    )
)

echo Running get_python.bat ... see python_setup_log.txt
echo ===== %DATE% %TIME% ===== > "%ENGINE_ROOT%python_setup_log.txt"
call "%ENGINE_ROOT%python\get_python.bat" >> "%ENGINE_ROOT%python_setup_log.txt" 2>&1
set GP_ERR=!ERRORLEVEL!

echo. >> "%ENGINE_ROOT%python_setup_log.txt"
echo get_python exit code: !GP_ERR! >> "%ENGINE_ROOT%python_setup_log.txt"

if not !GP_ERR!==0 (
    echo.
    echo  get_python.bat FAILED. Open this file in Notepad:
    echo  %ENGINE_ROOT%python_setup_log.txt
    echo.
    echo  Send the last 30 lines to Cursor chat for help.
    pause
    exit /b 1
)

echo.
echo  Testing python.cmd ...
call "%ENGINE_ROOT%python\python.cmd" --version
if errorlevel 1 (
    echo  python.cmd still failing - read python_setup_log.txt
    pause
    exit /b 1
)

echo.
echo  SUCCESS - Python for O3DE is ready.
echo  Next: double-click START_HERE.bat
echo.
pause
exit /b 0
