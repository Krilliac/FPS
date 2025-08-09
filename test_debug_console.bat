@echo off
echo ========================================
echo  Debug Console Output Test
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

echo [2/3] Debug Console Test Instructions:
echo.
echo ?? TO VIEW DEBUG OUTPUT:
echo   1. Open Visual Studio 2022
echo   2. Go to View ? Output
echo   3. In the "Show output from:" dropdown, select "Debug"
echo   4. You should see engine initialization messages
echo.
echo ?? WHAT TO LOOK FOR:
echo   • "ConsoleProcessManager::Initialize starting..."
echo   • Directory search paths being checked
echo   • Whether SparkConsole.exe was found or not
echo   • Engine subsystem initialization messages
echo.
echo ?? EXPECTED BEHAVIOR:
echo   • Debug output appears regardless of external console status
echo   • If SparkConsole.exe found: Both debug output AND external window
echo   • If SparkConsole.exe missing: Debug output only (fallback mode)
echo.

echo [3/3] Starting SparkEngine.exe...
echo.
echo ??  IMPORTANT: Keep Visual Studio Output window open to see debug messages!
echo.

Debug\SparkEngine.exe

echo.
echo Engine has exited. Check the Visual Studio Output window (Debug category)
echo for all the initialization and runtime messages.
echo.
pause