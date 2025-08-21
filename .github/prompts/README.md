# SparkEngine Development Prompts

Professional development prompts organized by category for the SparkEngine project.

## ?? **Category Structure**

### **Core-Setup** (Foundation & Build)
- [**01_project_overview.prompt.md**](Core-Setup/01_project_overview.prompt.md) - Complete project context and architecture overview
- [**02_code_refactor_rules.prompt.md**](Core-Setup/02_code_refactor_rules.prompt.md) - Code refactoring guidelines and standards
- [**03_build_system_ci.prompt.md**](Core-Setup/03_build_system_ci.prompt.md) - Build system, CI/CD, and deployment workflow

### **Engine-Systems** (Core Engine Components)
- [**01_engine_core.prompt.md**](Engine-Systems/01_engine_core.prompt.md) - Core engine architecture, ECS, and thread-safe operations
- [**02_graphics_rendering.prompt.md**](Engine-Systems/02_graphics_rendering.prompt.md) - DirectX 11 rendering pipeline and graphics optimization
- [**03_audio_system.prompt.md**](Engine-Systems/03_audio_system.prompt.md) - 3D spatial audio engine with procedural sound generation
- [**04_physics_collision.prompt.md**](Engine-Systems/04_physics_collision.prompt.md) - Physics simulation and collision detection
- [**05_collision_system.prompt.md**](Engine-Systems/05_collision_system.prompt.md) - Detailed CollisionSystem implementation guide

### **Gameplay-Specialized** (Game Features & Specialized Systems)
- [**01_game_logic.prompt.md**](Gameplay-Specialized/01_game_logic.prompt.md) - Game mechanics, player systems, and camera controls
- [**02_projectile_system.prompt.md**](Gameplay-Specialized/02_projectile_system.prompt.md) - Advanced projectile physics with object pooling
- [**03_shader_hlsl.prompt.md**](Gameplay-Specialized/03_shader_hlsl.prompt.md) - HLSL shader development and pipeline
- [**04_console_integration.prompt.md**](Gameplay-Specialized/04_console_integration.prompt.md) - 200+ command console system with real-time control

### **Tools-Development** (Editor & Development Tools)
- [**01_spark_editor.prompt.md**](Tools-Development/01_spark_editor.prompt.md) - Professional game engine editor with Dear ImGui
- [**02_assets_pipeline.prompt.md**](Tools-Development/02_assets_pipeline.prompt.md) - Asset processing, streaming, and management
- [**03_build_deployment.prompt.md**](Tools-Development/03_build_deployment.prompt.md) - Multi-platform build and deployment system
- [**04_debugging_profiling.prompt.md**](Tools-Development/04_debugging_profiling.prompt.md) - Advanced debugging tools and crash handling

### **Quality-Performance** (Optimization & QA)
- [**01_optimization.prompt.md**](Quality-Performance/01_optimization.prompt.md) - Performance optimization and profiling techniques
- [**02_testing_qa.prompt.md**](Quality-Performance/02_testing_qa.prompt.md) - Comprehensive testing and quality assurance

### **Advanced-Features** (Cutting-edge & Future Systems)
- [**01_networking_multiplayer.prompt.md**](Advanced-Features/01_networking_multiplayer.prompt.md) - Network architecture and multiplayer systems
- [**02_scripting_modding.prompt.md**](Advanced-Features/02_scripting_modding.prompt.md) - AngelScript integration and modding framework
- [**03_advanced_systems.prompt.md**](Advanced-Features/03_advanced_systems.prompt.md) - Cutting-edge features and specialized components

### **Documentation** (Project Documentation)
- [**01_docs_release_notes.prompt.md**](Documentation/01_docs_release_notes.prompt.md) - Documentation and release management

### **AI-Integration** (AI Assistant Tools)
- [**copilot-instructions.md**](AI-Integration/copilot-instructions.md) - GitHub Copilot configuration and guidelines

## ?? **Project Architecture**

### **Current Components**
1. **SparkEngine** - Core game engine with DirectX 11 rendering (3.17 MB)
2. **SparkEditor** - Professional editor built with Dear ImGui (233 KB)
3. **SparkConsole** - External console application for real-time debugging (405 KB)

### **Key Features**
- **Thread-Safe Operations**: All systems designed for multi-threading
- **Console Integration**: 200+ commands for real-time parameter control
- **Object Pooling**: Memory-efficient projectile and object management
- **Professional Debugging**: Crash handling, assertions, and external console
- **Modern C++20**: Cutting-edge language features and best practices
- **Cross-Platform**: Windows, Linux, macOS support via CMake

### **Build Status**
- ? **Zero Build Errors** in Debug and Release configurations
- ? **Zero Build Warnings** in all configurations
- ? **Clean Dependencies** with proper static library linking
- ? **Thread-Safe Console** with named pipe communication
- ? **Professional Architecture** following modern C++ practices

## ?? **Console Command Categories**

Each system prompt includes specific console commands that integrate with the 200+ command system:

- **Graphics**: `render_debug`, `shader_reload`, `graphics_stats`, `wireframe`
- **Audio**: `audio_debug`, `sound_play`, `spatial_audio`, `audio_volume`
- **Physics**: `physics_debug`, `collision_test`, `raycast_test`
- **Game**: `player_speed`, `noclip`, `god_mode`, `teleport`
- **Debug**: `assert_mode`, `crash_test`, `profile_start`, `memory_snapshot`
- **Editor**: `editor_theme`, `panel_toggle`, `asset_refresh`, `layout_save`
- **Build**: `build_clean`, `build_rebuild`, `build_config`, `build_status`
- **Assets**: `assets_refresh`, `assets_load`, `assets_memory_usage`

## ?? **Development Workflow**

1. **Project Context**: Start with [**Core-Setup/01_project_overview.prompt.md**](Core-Setup/01_project_overview.prompt.md)
2. **Build Environment**: Configure with [**Core-Setup/03_build_system_ci.prompt.md**](Core-Setup/03_build_system_ci.prompt.md)
3. **Choose Category**: Select relevant category folder for your development task
4. **Follow Guidelines**: Each prompt includes specific development guidelines
5. **Console Integration**: Test features using the integrated console system
6. **Performance Focus**: Maintain 60+ FPS target throughout development
7. **Quality Assurance**: Use Quality-Performance prompts for validation

## ?? **Quick Start Guide**

For new developers:
1. **Architecture Overview**: [**Core-Setup/01_project_overview.prompt.md**](Core-Setup/01_project_overview.prompt.md)
2. **Build Setup**: [**Core-Setup/03_build_system_ci.prompt.md**](Core-Setup/03_build_system_ci.prompt.md)
3. **Console Workflow**: [**Gameplay-Specialized/04_console_integration.prompt.md**](Gameplay-Specialized/04_console_integration.prompt.md)
4. **Core Systems**: Choose from Engine-Systems category
5. **Game Development**: Use Gameplay-Specialized prompts
6. **Quality Assurance**: Quality-Performance category for testing

## ?? **Contributing Guidelines**

When adding new features:
- **Category Organization**: Place prompts in appropriate category folders
- **Consistent Numbering**: Follow the numbering scheme within each category
- **Console Commands**: Include relevant debugging commands
- **Thread-Safety**: Ensure all code follows thread-safe practices
- **Performance Standards**: Maintain 60+ FPS and zero memory leaks
- **Documentation**: Update help systems and API documentation
- **Cross-Platform**: Test across all supported platforms
- **Zero-Warning Policy**: Ensure builds are completely clean

## ?? **Project Statistics**

- **Total Prompts**: 20 comprehensive development prompts across 8 categories
- **Console Commands**: 200+ commands across all systems
- **Build Targets**: 3 main executables (Engine, Editor, Console)
- **Supported Platforms**: Windows, Linux, macOS
- **Code Quality**: Zero warnings, zero errors, zero memory leaks
- **Performance Target**: 60+ FPS with full feature set active
- **Organization**: Clean category-based structure for easy navigation

## ? **Organization Complete**

The prompt system has been successfully organized into logical categories with:
- **No Duplicate Numbers**: Each category has its own clean numbering (01, 02, 03, 04)
- **Logical Grouping**: Related prompts are grouped together by functionality
- **Professional Structure**: Scalable organization for future expansion
- **Easy Navigation**: Clear hierarchy and cross-references between categories