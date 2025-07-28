Below is the updated `README.md` with enhanced build/setup instructions, including batch‐file argument details for specifying Visual Studio generators and other options.

```markdown
# Spark Engine

A modular, high-performance game engine built in modern C++ with DirectX 11, AngelScript scripting, and an integrated in-engine editor.

## Introduction

Spark Engine empowers developers to rapidly prototype and ship 3D first-person shooter games and beyond. It features a clean Entity-Component-System architecture, hot-reload scripting with AngelScript, a real-time visual editor, production-grade crash handling, and high-performance DirectX 11 rendering.

## Features

- **3D Graphics Rendering**  
  Real-time DirectX 11 pipeline with dynamic lighting, shadows, particle effects.
- **First-Person Camera**  
  Smooth mouse-look and WASD movement with jump, crouch, and zoom.
- **Input Handling**  
  Keyboard/mouse capture with external debug console overlay.
- **Modular Architecture**  
  ECS-driven design separating rendering, physics, audio, scripting, and gameplay.
- **Visual Editor** (optional)  
  ImGui-powered editor: scene hierarchy, inspector, asset browser, gizmos.
- **AngelScript Integration**  
  Unity-style hot-reload scripting, full debugging, C++ API exposure.
- **Crash Handling**  
  Minidumps, symbolized stack traces, screenshot capture, ZIP packaging, HTTP upload.
- **Shader System**  
  HLSL support with auto-recompilation and constant-buffer binding fixes.
- **Mesh Generation**  
  Built-in cube, plane, and sphere procedural meshes.
- **Cross-Platform Build**  
  CMake scripts for Windows, Linux, macOS; auto-discovers source files.

## Requirements

- **Windows**: Windows 10+, DirectX 11 GPU, Visual Studio 2019 or later  
- **Linux/macOS**: GCC/Clang, X11 (Linux) or Cocoa (macOS)

## Quick Start

### Windows (PowerShell)

1. Open PowerShell in the repo root.
2. Run `generate.bat` to configure CMake (no build):

   ```
   # Default: Debug + VS2022
   .\generate.bat

   # Specify configuration and generator:
   .\generate.bat release                 # Release + VS2022
   .\generate.bat debug                    # Debug + VS2022
   .\generate.bat -g "Visual Studio 16 2019" debug
   .\generate.bat -g "Ninja" release       # Generate Ninja files
   ```

3. Build with `build.ps1` (calls CMake build):

   ```
   .\build.ps1 -config Release -editor -angelscript
   .\build.ps1 -config Debug
   ```

   **Batch parameters**:
   - `-config `: Build configuration (default = Debug)
   - `-g ""`       : CMake generator (e.g. `"Visual Studio 17 2022"`, `"Ninja"`)
   - `-editor`                 : Enable in-engine editor
   - `-console`                : Enable external console overlay
   - `-angelscript`            : Enable AngelScript integration

### Linux/macOS (bash)

1. Ensure `build.sh` is executable:  
   ```
   chmod +x build.sh
   ```
2. Generate (configure) only:
   ```
   ./generate.sh                # Debug + Unix Makefiles
   ./generate.sh release        # Release + Unix Makefiles
   ./generate.sh -g Ninja       # Debug + Ninja
   ```
3. Build:
   ```
   ./build.sh debug             # Build Debug
   ./build.sh release           # Build Release
   ```

   **Shell parameters**:
   - `debug|release`           : Build type (default = Debug)
   - `-g `           : CMake generator (e.g. `Ninja`, `Unix Makefiles`)
   - `-E`                       : Disable editor
   - `-C`                       : Disable console overlay
   - `-A`                       : Disable AngelScript integration

## Running

After build, execute the engine:

```
# Windows
build/Release/SparkEngine.exe

# Linux/macOS
build/SparkEngine
```

## Controls

- **W/A/S/D**: Move  
- **Mouse**: Look  
- **Space**: Jump/Move up  
- **Ctrl**: Crouch/Move down  
- **Esc**: Release mouse / toggle menu  
- **Left Click**: (Re)capture mouse  
- **` (Backtick)**: Toggle debug console  

## Directory Structure

```
├── Source/
│   ├── Core/         # Framework & entry point
│   ├── Graphics/     # DirectX11 renderer
│   ├── Game/         # Game logic
│   ├── Camera/       # Camera controls
│   ├── Input/        # Input manager
│   ├── Utils/        # Helpers (Timer, File IO)
│   └── Scripting/    # AngelScript integration
├── ThirdParty/       # Submodules (EnTT, ImGui, AngelScript, stb, glm…)
├── Shaders/          # HLSL shader files
├── Resources/        # Assets (textures, models, audio)
├── generate.bat      # CMake configure (Windows)
├── build.ps1         # Build script (Windows)
├── generate.sh       # CMake configure (Linux/macOS)
├── build.sh          # Build script (Linux/macOS)
├── CMakeLists.txt    # Cross-platform CMake config
└── README.md         # Project overview
```

## License

MIT License.  
Commercial support and extended features available under separate terms.

Spark Engine delivers an **unrivaled blend of performance, flexibility, and ease-of-use**, enabling you to **ignite** your next game development adventure with confidence.