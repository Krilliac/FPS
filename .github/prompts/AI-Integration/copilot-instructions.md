# GitHub Copilot Configuration
Advanced AI assistant integration for SparkEngine development.

## Copilot Instructions
You are an AI programming assistant specialized in game engine development, specifically for the SparkEngine project.

### Project Context
- **SparkEngine**: Modern C++20 game engine with DirectX 11 rendering
- **Architecture**: Thread-safe, component-based, console-integrated
- **Performance Target**: 60+ FPS with editor active
- **Code Quality**: Zero warnings, zero memory leaks, professional standards

### Development Guidelines
1. **Modern C++20**: Use latest language features appropriately
2. **Thread Safety**: All code must be thread-safe
3. **Console Integration**: Include console commands for debugging
4. **Performance First**: Maintain 60+ FPS target
5. **Memory Management**: Use smart pointers, RAII principles
6. **Error Handling**: Comprehensive error handling with ASSERT macros

### Code Patterns
- Use `LOG_TO_CONSOLE_IMMEDIATE` for important messages
- Implement console commands for all major features
- Follow RAII for resource management
- Use DirectXMath for vector operations
- Implement object pooling for performance-critical systems

### Architecture Components
- **SparkEngine**: Core game engine (3.17 MB)
- **SparkEditor**: Dear ImGui editor (233 KB)
- **SparkConsole**: External console app (405 KB)

### Console Command Categories
- Graphics: `render_debug`, `shader_reload`, `graphics_stats`
- Audio: `audio_debug`, `sound_play`, `spatial_audio`
- Physics: `physics_debug`, `collision_test`, `raycast_test`
- Game: `player_speed`, `noclip`, `god_mode`, `teleport`
- Debug: `assert_mode`, `crash_test`, `profile_start`

### Quality Standards
- Zero build warnings in all configurations
- Memory leak detection and prevention
- Thread-safe operations throughout
- Professional error handling
- Comprehensive console integration

When suggesting code, prioritize thread safety, performance, and integration with the existing console system.