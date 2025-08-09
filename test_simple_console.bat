@echo off
echo ========================================
echo  Simple Console System Test
echo ========================================
echo.

echo [1/3] Building the project...
cd build
cmake --build . --config Debug
if %ERRORLEVEL% neq 0 (
    echo ? BUILD FAILED!
    pause
    exit /b 1
)
echo ? Build successful!
echo.

echo [2/3] Starting Spark Engine with Simple Console...
echo.
echo ?? SIMPLE CONSOLE SYSTEM:
echo   • Separate console window (automatically opens)
echo   • All engine output appears in both:
echo     - Console window (interactive)
echo     - Visual Studio Output window (Debug category)
echo   • Engine Overlay Console: Press ~ (tilde) in main window
echo.
echo ?? AVAILABLE COMMANDS:
echo   help              - Show all available commands
echo   info              - Engine information  
echo   fps               - Current FPS and performance
echo   graphics_info     - Graphics engine status
echo   engine_status     - Complete system status
echo   memory_info       - System memory information
echo   test_engine       - Test engine responsiveness
echo   test_debug        - Test debug console output
echo   toggle_console    - Toggle overlay console
echo   window_info       - Window dimensions
echo   test              - Test all log levels (INFO/WARNING/ERROR/SUCCESS)
echo   clear             - Clear console
echo   history           - Show command history
echo   echo <message>    - Echo a message
echo   exit              - Exit application
echo.
echo ?? TECHNICAL FEATURES:
echo   • Simple, reliable console (no complex pipes or processes)
echo   • Always works - no external dependencies
echo   • Colored output for different log types
echo   • Command history and logging
echo   • Thread-safe operation
echo   • Both console window AND debug output
echo.
echo Starting engine now...
echo.

Debug\SparkEngine.exe

echo.
echo [3/3] Engine has exited.
echo.
echo ?? TEST RESULTS:
if %ERRORLEVEL% equ 0 (
    echo ? Engine exited normally
) else (
    echo ? Engine exited with error code %ERRORLEVEL%
)

echo.
echo ?? If you saw a separate console window with engine output,
echo    then the simple console system is working perfectly!
echo.
echo ?? To verify it's working:
echo    1. Look for the "Spark Engine Console (Simple & Reliable)" window
echo    2. Try typing commands like 'help', 'info', 'fps'
echo    3. Check Visual Studio Output window (Debug category) for messages
echo.
pause