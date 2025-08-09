# SparkEngine Startup Troubleshooting Guide

## Quick Test Steps

### 1. First, test if SparkConsole works standalone:
```batch
cd build\bin
SparkConsole.exe
```
- Should show "Spark Engine Console v1.0.0"
- Try typing: `diag`, `help`, `status`
- Type `exit` to quit

### 2. Test SparkEngine with debug output:
```batch
cd build\bin
SparkEngine.exe
```

## Expected Results

### If Working Correctly:
1. **SparkEngine window appears** (DirectX 11 window with blue background)
2. **SparkConsole window appears automatically** 
3. **Console shows connection messages** like:
   ```
   [INFO] SparkConsole system initialized
   [INFO] External console connection established
   [INFO] All engine systems initialized
   ```
4. **You can type commands** in SparkConsole window:
   - `help` - Shows available commands
   - `engine_status` - Shows all engine systems status
   - `fps` - Shows current frame rate
   - `graphics_info` - Shows graphics engine info

### If NOT Working:

#### SparkConsole opens but shows "standalone mode":
- **Cause**: SparkEngine failed to launch or crashed during startup
- **Check**: Look in Visual Studio Output window for error messages
- **Debug**: Run `debug_engine.bat` script for detailed diagnostics

#### SparkConsole shows blank black screen:
- **Cause**: Console input/output redirection failed
- **Fix**: Close both programs, ensure both .exe files are in same directory
- **Try**: Run SparkConsole.exe standalone first to test

#### Neither program starts:
- **Cause**: Missing dependencies or build issues
- **Fix**: Rebuild project: `cmake --build build --config Debug`
- **Check**: Ensure both SparkEngine.exe and SparkConsole.exe exist in build\bin\

#### SparkEngine crashes immediately:
- **Cause**: Graphics initialization failed or missing DirectX
- **Check**: Visual Studio Output window for assertion failures
- **Try**: Update graphics drivers, ensure DirectX 11 is available

## Debug Commands to Try

Once connected, test these commands in SparkConsole:

### Basic Commands:
- `help` - List all commands
- `engine_status` - Show all systems status
- `console_status` - Show connection info
- `diag` - SparkConsole diagnostics

### Engine Information:
- `fps` - Current frame rate
- `graphics_info` - Graphics engine status  
- `memory_info` - System memory usage

### Test Commands (use carefully):
- `test_assert` - Triggers test assertion
- `crash_mode on/off` - Toggle crash dump generation

## Visual Studio Debug Output

Watch the Output window in Visual Studio for these messages:

### Success Messages:
```
ConsoleProcessManager::Initialize starting...
SparkEngine executable directory: [path]
SUCCESS: Found SparkConsole.exe at: [path]
SUCCESS: External console system initialized
```

### Error Messages:
```
WARNING: SparkConsole.exe not found in any location
ERROR: Failed to launch SparkConsole.exe
```

## Common Issues and Fixes

### Issue: "SparkConsole.exe not found"
**Solution**: Ensure both executables are built and in same directory:
```batch
cmake --build build --config Debug
dir build\bin\*.exe
```

### Issue: Graphics initialization failed
**Solution**: 
1. Update graphics drivers
2. Check if DirectX 11 is available
3. Try running as administrator

### Issue: SparkEngine window appears but is unresponsive
**Solution**:
1. Check CPU usage (should be active, not 0%)
2. Try pressing ~ key to toggle console
3. Check for assertion failures in debug output

### Issue: Console connects but commands don't work
**Solution**:
1. Type `engine_status` to check if systems are initialized
2. Verify SparkEngine main loop is running
3. Check Visual Studio debug output for errors

## Memory Usage Reference

- **SparkEngine running properly**: ~25-30 MB
- **SparkEngine crashed/failed**: ~10-15 MB  
- **SparkConsole standalone**: ~5-8 MB

If SparkEngine shows low memory usage (under 20MB), it likely crashed during initialization.

## Files to Check

Ensure these files exist in `build\bin\`:
- `SparkEngine.exe` - Main engine executable
- `SparkConsole.exe` - Debug console executable

## Getting More Debug Info

1. **Enable debug output**: Run from Visual Studio with debugger attached
2. **Check Windows Event Viewer**: Look for application crashes
3. **Use debug script**: Run `debug_engine.bat` for automated testing
4. **Console logs**: SparkConsole saves connection info in its window

## Last Resort Fixes

1. **Clean rebuild**:
   ```batch
   rmdir /s build
   cmake -B build
   cmake --build build --config Debug
   ```

2. **Check antivirus**: Add build directory to antivirus exceptions

3. **Run as administrator**: Some systems require elevated privileges for pipe communication

4. **Verify dependencies**: Ensure Visual C++ Runtime is installed