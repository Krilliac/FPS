# Spark Engine

A modular, high-performance C++ game engine for 3D titles, featuring DirectX 11 rendering, Lua scripting, and an integrated ImGui editor. Built with CMake for Windows, Linux, and macOS.

## Key Features

- **DirectX 11 Renderer**: Dynamic lighting, shadows, post-processing, particles  
- **ECS Architecture**: EnTT-based entity-component system  
- **Lua Scripting**: Sol2 integration for hot-reloadable gameplay logic  
- **ImGui Editor**: Scene hierarchy, inspector, asset browser, gizmos  
- **PBR Materials**: Physically based rendering with full material workflow  
- **Multi-threaded Rendering**: Deferred contexts and command queues  
- **Physics (Optional)**: PhysX integration for rigid bodies and colliders  
- **Asset Pipeline**: Async loading of textures and meshes via `AssetManager`

## Quick Start

```
git clone --recurse-submodules https://github.com//SparkEngine.git
cd SparkEngine

# Initialize submodules
git submodule update --init --recursive

# Generate build files (Windows)
cmake -B build -S . -G "Visual Studio 17 2022" -A x64

# Generate build files (Linux/macOS)
cmake -B build -S . -G Ninja

# Build
cmake --build build --config Release

# Run
# Windows
.\build\Release\SparkEngine.exe
# Linux/macOS
./build/SparkEngine
```

## Build & Setup

Dependencies are included as submodules under `ThirdParty/`. No external installers required.

1. Clone and update submodules:
   ```
   git submodule update --init --recursive
   ```
2. Generate build files:
   - Windows:  
     ```
     cmake -B build -S . -G "Visual Studio 17 2022" -A x64
     ```
   - Linux/macOS:  
     ```
     cmake -B build -S . -G Ninja
     ```
3. Build:
   ```
   cmake --build build --config Release
   ```
4. Run:
   - Windows: `.\build\Release\SparkEngine.exe`  
   - Linux/macOS: `./build/SparkEngine`

### CMakeLists.txt Snippet

```cmake
add_subdirectory(ThirdParty/entt)
add_subdirectory(ThirdParty/imgui)
add_subdirectory(ThirdParty/sol2)
add_subdirectory(ThirdParty/PhysX)       # optional
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ThirdParty/stb)

file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_SOURCE_DIR}/Spark Engine/Source/*.cpp"
    "${CMAKE_SOURCE_DIR}/Spark Engine/Source/*.h"
)

add_executable(SparkEngine WIN32 ${ENGINE_SOURCES} "Spark Engine/SparkEngine.rc")
target_include_directories(SparkEngine PRIVATE "Spark Engine/Source")
target_link_libraries(SparkEngine PRIVATE
    EnTT::EnTT
    imgui
    sol2::sol2
    stb_image
    d3d11 dxgi d3dcompiler winmm
)

if(TARGET PhysX)
    target_link_libraries(SparkEngine PRIVATE PhysX::PhysX)
    target_compile_definitions(SparkEngine PRIVATE ENABLE_PHYSX)
endif()
```

## Controls

| Input       | Action                 |
|-------------|------------------------|
| W/A/S/D     | Move                   |
| Mouse       | Look                   |
| Space       | Jump                   |
| Ctrl        | Crouch                 |
| Esc         | Release mouse/toggle UI|
| Backtick (`)| Toggle console         |

## Architecture Overview

```
Rendering      Physics         Audio
─────────────  ─────────────   ─────────────
GraphicsEngine PhysicsSystem   AudioSystem
ShaderManager  Collision       Mixer
MeshManager    RigidBodies     Effects

Scripting      Input & UI      Core & Utils
────────────   ─────────────   ─────────────
Sol2 (Lua)     InputManager    Engine
ScriptSystem   ConsoleOverlay  Timer
Bindings       ImGui Editor    FileSystem
```

## Directory Layout

```
.
├── Spark Engine/        # Source with space in folder
│   ├── Core/            # Framework, entry point
│   ├── ECS/             # EntityRegistry, SystemManager, Components
│   ├── Graphics/        # Renderer, Shaders, Threading
│   ├── Physics/         # PhysX integration
│   ├── Scripting/       # Lua scripting system
│   ├── Editor/          # ImGui windows
│   ├── Assets/          # AssetManager, TextureAsset, MeshAsset
│   └── Game/            # SparkEngineGame example
├── Shaders/             # HLSL files
├── ThirdParty/          # entt, imgui, sol2, PhysX, stb
├── SparkEngine.rc       # Windows resource script
├── generate.bat/.sh     # CMake configure scripts
├── build.ps1/.sh        # Build scripts
├── CMakeLists.txt       # Build configuration
└── README.md            # This file
```

## Configuration Options

| Option            | Default | Description                    |
|-------------------|---------|--------------------------------|
| ENABLE_EDITOR     | ON      | ImGui editor                   |
| ENABLE_LUA        | ON      | Lua scripting                  |
| ENABLE_PHYSX      | OFF     | PhysX physics (optional)       |
| ENABLE_VULKAN     | OFF     | Vulkan renderer (future)       |
| USE_STATIC_RUNTIME| ON      | Static CRT on MSVC (/MT)       |

Customize with:
```
cmake -B build -S . -DENABLE_PHYSX=ON
```

## License

MIT License. Commercial support available.