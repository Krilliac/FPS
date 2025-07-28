# Spark Engine

A modular, high-performance game engine built in modern C++ with DirectX 11, AngelScript scripting, and an integrated in-engine editor.

## Introduction

Spark Engine empowers developers to rapidly prototype and ship 3D first-person shooter games and beyond. It features a clean Entity-Component-System architecture, hot-reload scripting with AngelScript, a real-time visual editor, production-grade crash handling, and high-performance DirectX 11 rendering.

## Features

- **3D Graphics Rendering**  
  Real-time DirectX 11 pipeline with support for dynamic lighting, shadows, and particle effects.
- **First-Person Camera**  
  Smooth mouse-look and WASD movement with jump, crouch, and zoom controls.
- **Input Handling**  
  Keyboard and mouse capture routines with console overlay for runtime commands.
- **Modular Architecture**  
  ECS-driven design separating rendering, physics, audio, scripting, and gameplay logic.
- **Visual Editor** (optional)  
  Built-in ImGui-powered editor with scene hierarchy, inspector, asset browser, and gizmos.
- **AngelScript Integration**  
  Unity-style hot-reload scripting, full debugging support, and C++ API exposure.
- **Crash Handling**  
  Automatic minidumps, symbolized stack traces, screenshot capture, ZIP packaging, and HTTP upload.
- **Shader System**  
  HLSL support with auto-recompilation and constant-buffer binding fixes.
- **Mesh Generation**  
  Built-in cube, plane, and sphere meshes with runtime procedural geometry.
- **Cross-Platform Build**  
  CMake scripts for Windows, Linux, and macOS; auto-discovers source files.

## Requirements

- **Windows**: Windows 10 or later, DirectX 11 GPU, Visual Studio 2019+  
- **Linux/macOS**: GCC/Clang toolchain, X11 (Linux) or Cocoa (macOS)

## Quick Start

### Windows (PowerShell)

```powershell
.\build.ps1 -config Release -editor -angelscript
.\build.ps1 -config Debug   -editor -angelscript
```

### Linux/macOS (bash)

```bash
chmod +x build.sh
./build.sh release    # Release build, editor & AngelScript enabled
./build.sh debug      # Debug build, editor & AngelScript enabled
```

## Running

After building, the executable is located in the `build/` directory:

```bash
# Windows
build/Release/SparkEngine.exe

# Linux/macOS
build/SparkEngine
```

## Controls

- **W/A/S/D**: Move forward/left/back/right  
- **Mouse**: Look around  
- **Space**: Jump/Move up  
- **Ctrl**: Crouch/Move down  
- **Esc**: Release mouse cursor / toggle menu  
- **Left Click**: Re-capture mouse for SparkEngine controls  
- **` (Backtick)**: Toggle external debug console  

## Directory Structure

```
├── Source/
│   ├── Core/         # Entry point, application framework
│   ├── Graphics/     # DirectX11 rendering engine
│   ├── Game/         # Game logic & objects
│   ├── Camera/       # SparkEngine camera system
│   ├── Input/        # Input manager
│   ├── Utils/        # Utility classes (Timer, File IO, etc.)
│   └── Scripting/    # AngelScript integration
├── ThirdParty/       # Submodules (EnTT, ImGui, AngelScript, stb, glm, etc.)
├── Shaders/          # HLSL shader source files
├── Resources/        # Textures, models, audio assets
├── build.ps1         # Windows build script
├── build.sh          # Linux/macOS build script
├── CMakeLists.txt    # Cross-platform CMake project
└── README.md         # Project overview (this file)
```

## License

MIT License.  
Optionally purchase commercial support and extended features under a separate license—visit our website for details.

Spark Engine delivers an **unrivaled blend of performance, flexibility, and ease-of-use**, enabling you to **ignite** your next game development adventure with confidence.
