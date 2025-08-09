# SparkEngine - SparkConsole Connection Troubleshooting Guide

## Overview
SparkEngine and SparkConsole communicate via Windows named pipes for real-time debugging and command execution.

## Expected Behavior
1. When SparkEngine.exe starts, it automatically launches SparkConsole.exe
2. SparkConsole opens in a separate window with a command prompt
3. Log messages from SparkEngine appear in the SparkConsole window
4. Commands typed in SparkConsole are executed by SparkEngine
5. Results are displayed back in the SparkConsole window

## Build Requirements
Both executables must be built and placed in the same directory:
- `build/bin/SparkEngine.exe`
- `build/bin/SparkConsole.exe`

## Testing the Connection

### Quick Test Commands
In the SparkConsole window, try these commands:
- `help` - Shows all available commands
- `info` - Shows engine information
- `fps` - Shows current frame rate
- `console_status` - Shows connection status
- `memory_info` - Shows system memory information

### Debug Commands (Use with caution)
- `test_assert` - Triggers a test assertion
- `crash_mode on/off` - Toggles crash dump generation
- `test_null_access` - Triggers a crash (for testing crash handling)

## Troubleshooting

### Problem: SparkConsole window doesn't appear
**Causes:**
- SparkConsole.exe not found in the same directory as SparkEngine.exe
- Build failed for SparkConsole
- Permission issues

**Solutions:**
1. Check that both .exe files exist in `build/bin/`
2. Rebuild the project: `cmake --build build --config Debug`
3. Run as administrator if needed
4. Check the Visual Studio Output window for error messages

### Problem: SparkConsole window appears but shows "Running in standalone mode"
**Causes:**
- Pipe communication failed to establish
- SparkEngine crashed during startup
- Antivirus blocking pipe communication

**Solutions:**
1. Check that SparkEngine.exe is still running
2. Restart both applications
3. Add SparkEngine.exe and SparkConsole.exe to antivirus exceptions
4. Check Windows Event Viewer for application errors

### Problem: Commands typed in SparkConsole don't work
**Causes:**
- Connection lost between processes
- SparkEngine main loop stopped
- Command not registered properly

**Solutions:**
1. Type `console_status` to check connection
2. Try basic commands like `help` first
3. Check SparkEngine window is responsive
4. Restart both applications

### Problem: No log messages appear in SparkConsole
**Causes:**
- Logging system not properly initialized
- Pipe communication one-way only
- Buffer issues

**Solutions:**
1. Check that SparkEngine completed initialization
2. Look for log messages in Visual Studio Output window
3. Try triggering actions in SparkEngine (resize window, etc.)

## Advanced Debugging

### Manual Testing
1. Open Command Prompt
2. Navigate to `build/bin/`
3. Run `SparkConsole.exe` manually
4. Run `SparkEngine.exe` in another command prompt
5. Check if they connect properly

### Development Tips
- Use `LOG_TO_CONSOLE()` macro in SparkEngine code to send messages to SparkConsole
- Messages also appear in OutputDebugString (visible in Visual Studio Output)
- Console connection status is logged during SparkEngine startup

### Performance Notes
- Pipe communication adds minimal overhead
- Console can be disabled by not including SparkConsole.exe
- Fallback logging to OutputDebugString when console not available

## File Locations
- Source: `SparkConsole/src/`
- Build output: `build/bin/SparkConsole.exe`
- SparkEngine console manager: `Spark Engine/Source/Utils/ConsoleProcessManager.cpp`