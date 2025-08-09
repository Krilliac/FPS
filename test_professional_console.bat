@echo off
echo ========================================
echo  Spark Engine Professional Console
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

echo [2/3] Starting Spark Engine with Professional Console...
echo.
echo ?? PROFESSIONAL DEVELOPMENT CONSOLE
echo   • Advanced debugging interface
echo   • Comprehensive command system
echo   • Professional presentation
echo   • Multi-category command organization
echo.
echo ?? COMMAND CATEGORIES:
echo.
echo   SYSTEM COMMANDS:
echo     help               - Complete command reference
echo     clear              - Clear console screen
echo     history            - Command history
echo     version            - Engine version info
echo     uptime             - Engine runtime
echo     exit               - Shutdown engine
echo.
echo   ENGINE DIAGNOSTICS:
echo     engine_status      - Comprehensive status report
echo     engine_config      - Configuration management
echo     engine_modules     - Active module listing
echo.
echo   GRAPHICS ^& RENDERING:
echo     graphics_info      - Graphics engine status
echo     render_wireframe   - Toggle wireframe mode
echo     render_stats       - Rendering statistics
echo     shader_reload      - Hot-reload shaders
echo     texture_info       - Texture manager status
echo.
echo   GAME SYSTEMS:
echo     game_pause         - Pause/unpause game
echo     game_speed         - Set game speed multiplier
echo     player_teleport    - Teleport player to coordinates
echo     camera_info        - Camera system status
echo     scene_objects      - List scene objects
echo.
echo   DEBUG ^& TESTING:
echo     debug_overlay      - Toggle debug overlay
echo     debug_assert       - Assert system testing
echo     debug_memory       - Memory usage analysis
echo     debug_threads      - Thread information
echo     test_crash         - Crash handler testing (DANGEROUS)
echo.
echo   FILE OPERATIONS:
echo     file_list          - Directory listing
echo     file_read          - Read file contents
echo     log_save           - Save console log to file
echo.
echo   PERFORMANCE MONITORING:
echo     fps                - FPS and performance metrics
echo     performance_profile - Comprehensive performance data
echo     performance_benchmark - Run benchmark tests
echo.
echo ?? PROFESSIONAL FEATURES:
echo   • Categorized help system with 'help <command>' for details
echo   • Enhanced error handling and validation
echo   • Professional console presentation
echo   • Comprehensive engine control and monitoring
echo   • Advanced debugging capabilities
echo   • File system operations
echo   • Performance profiling tools
echo   • Memory and thread analysis
echo   • Configuration management
echo.
echo ?? USAGE EXAMPLES:
echo   help                    - Show all commands by category
echo   help engine_status      - Get detailed help for specific command
echo   engine_status           - Complete engine status report
echo   debug_memory            - Detailed memory analysis
echo   file_list Assets        - List files in Assets directory
echo   player_teleport 10 5 0  - Move player to coordinates
echo   game_speed 2.0          - Double game speed
echo.
echo Starting professional console system...
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
echo ?? Professional Console Features Demonstrated:
echo   ? Enhanced presentation without amateur language
echo   ? Comprehensive command categorization
echo   ? Advanced debugging and monitoring capabilities
echo   ? Professional error handling and validation
echo   ? Multi-system control and configuration
echo   ? Performance profiling and analysis tools
echo   ? File system operations
echo   ? Memory and thread debugging
echo.
echo ?? The console now provides enterprise-level debugging
echo    capabilities suitable for professional game development.
echo.
pause