@echo off
echo Testing SparkEngine Console Connection
echo =====================================
echo.

echo Checking if executables exist...
if exist "build\bin\SparkEngine.exe" (
    echo [OK] SparkEngine.exe found
) else (
    echo [ERROR] SparkEngine.exe not found in build\bin\
    goto :error
)

if exist "build\bin\SparkConsole.exe" (
    echo [OK] SparkConsole.exe found
) else (
    echo [ERROR] SparkConsole.exe not found in build\bin\
    goto :error
)

echo.
echo Checking shader files...
if exist "build\bin\Shaders\HLSL\BasicVS.hlsl" (
    echo [OK] BasicVS.hlsl found
) else (
    echo [WARNING] BasicVS.hlsl not found - this may cause white screen
)

if exist "build\bin\Shaders\HLSL\BasicPS.hlsl" (
    echo [OK] BasicPS.hlsl found
) else (
    echo [WARNING] BasicPS.hlsl not found - this may cause white screen
)

echo.
echo Starting SparkEngine with console debugging...
echo Press Ctrl+C to stop
echo.

cd build\bin
SparkEngine.exe

goto :end

:error
echo.
echo Build the project first with: cmake --build build --config Debug
echo.
pause

:end