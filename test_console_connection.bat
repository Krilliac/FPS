@echo off
echo Testing SparkEngine and SparkConsole connection...
echo.

echo Building project...
cmake --build build --config Debug
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo.

echo Checking if executables exist...
if not exist "build\bin\SparkEngine.exe" (
    echo ERROR: SparkEngine.exe not found in build\bin\
    pause
    exit /b 1
)

if not exist "build\bin\SparkConsole.exe" (
    echo ERROR: SparkConsole.exe not found in build\bin\
    pause
    exit /b 1
)

echo Both executables found!
echo.

echo Starting SparkEngine (this should automatically launch SparkConsole)...
echo You should see:
echo 1. SparkEngine main window
echo 2. SparkConsole debug window with log messages
echo 3. Try typing commands in the SparkConsole window like:
echo    - help
echo    - info
echo    - fps
echo    - console_status
echo    - test_assert (be careful - this will trigger an assertion)
echo.

cd build\bin
start SparkEngine.exe

echo SparkEngine started. Check for the SparkConsole window!
echo Press any key to exit this script...
pause > nul