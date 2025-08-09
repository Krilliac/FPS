@echo off
echo =================================================
echo SparkEngine Debug and Testing Script
echo =================================================
echo.

echo Step 1: Checking build directory structure...
echo.

if not exist "build" (
    echo ERROR: build directory not found!
    echo Please run: cmake -B build
    echo Then: cmake --build build --config Debug
    pause
    exit /b 1
)

cd build

if not exist "bin" (
    echo ERROR: bin directory not found in build!
    echo Running build to create it...
    cmake --build . --config Debug
)

echo Step 2: Checking for executables...
echo.

if not exist "bin\SparkEngine.exe" (
    echo ERROR: SparkEngine.exe not found!
    echo Building project...
    cmake --build . --config Debug
    if not exist "bin\SparkEngine.exe" (
        echo Build failed! Check for compilation errors.
        pause
        exit /b 1
    )
)

if not exist "bin\SparkConsole.exe" (
    echo ERROR: SparkConsole.exe not found!
    echo Building project...
    cmake --build . --config Debug
    if not exist "bin\SparkConsole.exe" (
        echo Build failed! Check for compilation errors.
        pause
        exit /b 1
    )
)

echo Both executables found!
echo.

echo Step 3: Testing SparkConsole standalone...
echo.
cd bin

echo Starting SparkConsole in standalone mode for 10 seconds...
echo Try typing: diag, help, status, echo hello
timeout /t 2 /nobreak > nul

start /wait /b cmd /c "echo Type 'diag' then 'exit' to test | SparkConsole.exe"

echo.
echo Step 4: Testing SparkEngine (this should auto-launch SparkConsole)...
echo.

echo Starting SparkEngine...
echo - SparkEngine window should appear
echo - SparkConsole debug window should also appear
echo - Check SparkConsole for log messages from SparkEngine
echo - Try typing commands in SparkConsole window
echo.

start SparkEngine.exe

echo SparkEngine started! Check for both windows.
echo.
echo If SparkConsole window shows log messages from SparkEngine, the connection is working!
echo If SparkConsole is blank or shows "standalone mode", there may be a connection issue.
echo.
echo Press any key to exit this script...
pause > nul