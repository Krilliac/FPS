@echo off
echo ========================================
echo  Spark Engine Console System Test
echo ========================================
echo.

echo [1/4] Building the project...
cd build
cmake --build . --config Debug
if %ERRORLEVEL% neq 0 (
    echo ? BUILD FAILED!
    pause
    exit /b 1
)
echo ? Build successful!
echo.

echo [2/4] Checking for SparkConsole.exe...
if exist "Debug\SparkConsole.exe" (
    echo ? SparkConsole.exe found
) else (
    echo ? SparkConsole.exe not found - external console may not work
)
echo.

echo [3/4] Starting Spark Engine...
echo.
echo ?? UNIFIED CONSOLE SYSTEM:
echo   • Engine Overlay Console: Press ~ (tilde) in main window
echo   • External Console: SparkConsole.exe window (automatic)
echo.
echo ?? AVAILABLE COMMANDS:
echo   help              - Show all available commands
echo   info              - Engine information
echo   fps               - Current FPS and performance
echo   graphics_info     - Graphics engine status
echo   engine_status     - Complete system status
echo   memory_info       - System memory information
echo   test_engine       - Test engine responsiveness
echo   toggle_console    - Toggle overlay console
echo   window_info       - Window dimensions
echo   assert_test       - Test assertion system
echo   crash_test        - Test crash handling
echo   quit              - Exit application
echo.
echo ?? TECHNICAL FEATURES:
echo   • Multithreaded non-blocking console I/O
echo   • Rate-limited logging to prevent spam
echo   • Colored output (INFO/WARNING/ERROR/SUCCESS/DEBUG)
echo   • Pipe-based IPC with external console
echo   • Thread-safe command and message queues
echo   • Graceful fallback if external console unavailable
echo.
echo Starting engine now...
echo.

Debug\SparkEngine.exe

echo.
echo [4/4] Engine has exited.
echo.
echo ?? TEST RESULTS:
if %ERRORLEVEL% equ 0 (
    echo ? Engine exited normally
) else (
    echo ? Engine exited with error code %ERRORLEVEL%
)

echo.
echo ?? If you saw both the main engine window and SparkConsole.exe window,
echo    then the unified console system is working correctly!
echo.
pause