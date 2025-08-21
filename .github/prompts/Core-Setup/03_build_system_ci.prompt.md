# Build System and CI/CD Tasks
Professional build configuration and continuous integration.

## Build System Configuration
1. **CMake Setup**
   - Cross-platform build support (Windows, Linux, macOS)
   - Proper dependency management (TinyObjLoader, MINIZ, Dear ImGui)
   - Debug/Release/Profile configurations
   - Static analysis integration
   - Use `cmake -B build -G "Visual Studio 17 2022" -A x64` after inserting new files
   - **Never insert new folders or files directly into ./build directory**

2. **Compiler Settings**
   - Modern C++20 standard enforcement
   - Warnings as errors (-Wall -Werror) in development builds
   - Optimization flags per configuration
   - Consistent runtime library usage (MultiThreaded DLL)
   - Address sanitizer integration for debugging

## Current Build Status
- ? **SparkEngine.exe** - Core game engine (3.17 MB)
- ? **SparkEditor.exe** - Professional editor (233 KB) 
- ? **SparkConsole.exe** - External console (405 KB)
- ? **Zero Build Errors** in Debug and Release
- ? **Zero Build Warnings** in all configurations
- ? **Clean Dependencies** with conditional compilation

## Build Dependencies
- **TinyObjLoader**: Static library for OBJ model loading
- **MINIZ**: Compression library for asset packaging
- **Dear ImGui**: Editor UI framework
- **DirectX 11**: Graphics rendering API
- **Windows SDK**: Platform-specific functionality

## CI/CD Pipeline
1. **Automated Builds**
   - Trigger builds on code commits
   - Multi-platform build verification
   - Performance regression testing
   - Artifact generation and deployment

2. **Quality Gates**
   - Zero compilation errors policy
   - Zero warning tolerance in CI
   - Memory leak detection tests
   - Performance benchmark validation
   - Cross-platform compatibility checks

## Build Commands
```bash
# Clean configuration
cmake .. -DENABLE_NETWORKING=OFF

# Debug build
cmake --build . --config Debug

# Release build  
cmake --build . --config Release

# Clean rebuild
cmake --build . --config Debug --clean-first
```

## Console Integration
- `build_clean` - Clean build artifacts
- `build_rebuild` - Force rebuild all targets
- `build_config <Debug/Release>` - Switch build configuration
- `build_tests` - Run test suite
- `build_status` - Show current build information

## Quality Assurance
- **Code Coverage**: Comprehensive test coverage reporting
- **Static Analysis**: clang-tidy, PVS-Studio integration
- **Memory Analysis**: Leak detection and allocation profiling
- **Performance Benchmarks**: Automated performance regression testing

## Build Optimization
- **Parallel Compilation**: Multi-core build acceleration
- **Incremental Builds**: Only rebuild changed components
- **Precompiled Headers**: Faster compilation times
- **Unity Builds**: Reduced compilation overhead
- **Build Caching**: Shared build cache for team development

## Deployment Pipeline
- **Automated Packaging**: Create distribution packages
- **Code Signing**: Digital signature for executables
- **Update System**: Incremental update deployment
- **Platform Distribution**: Steam, Epic Games Store integration
- **Quality Validation**: Automated testing before deployment

## Error Resolution
Recent fixes implemented:
- ? **CURL Dependencies**: Conditional compilation for networking
- ? **TinyObjLoader**: Proper static library configuration
- ? **Unicode Issues**: Fixed character encoding warnings
- ? **Runtime Libraries**: Consistent MSVC runtime linking
- ? **Symbol Conflicts**: Eliminated duplicate definitions

## Development Guidelines
- Never insert files directly into `./build` directory
- Always regenerate CMake after adding new source files
- Test both Debug and Release configurations
- Maintain zero-warning policy for clean builds
- Use consistent coding standards across all platforms