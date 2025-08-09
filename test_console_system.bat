@echo off
echo Testing Spark Engine Console System
echo =====================================
echo.

echo Building the project...
cd build
cmake --build . --config Debug
if %ERRORLEVEL% neq 0 (
    echo BUILD FAILED!
    pause
    exit /b 1
)

echo.
echo Build successful! Starting SparkEngine.exe...
echo.
echo Console Commands Available:
echo   help              - Show all available commands
echo   info              - Show engine information
echo   fps               - Show current FPS
echo   graphics_info     - Show graphics engine information
echo   engine_status     - Show complete engine status
echo   memory_info       - Show system memory information
echo   test_engine       - Test if engine is responsive
echo   clear             - Clear the console
echo   exit              - Exit the application
echo.
echo Press tilde (~) key in the main window to toggle the engine console
echo.

Debug\SparkEngine.exe

echo.
echo SparkEngine has exited.
pause