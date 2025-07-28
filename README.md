# Spark Engine

A modular, high-performance C++ game engine for 3D FPS titles and beyond, featuring DirectX 11 rendering, AngelScript scripting, and an integrated editor.

This Engine is still VERY young, fresh and all the issues are being worked out while slowly providing a fully featured game project/engine. Things may break or not compile/work at all.

## 🌟 Key Features

### 🎨 Rendering & Camera
- **DirectX 11 Pipeline**: Dynamic lighting, shadows, post-processing, particle effects  
- **First-Person Camera**: Smooth mouselook, WASD movement, jump, crouch, zoom  

### 🎮 Gameplay & Input
- **ECS Architecture**: Clean separation of rendering, physics, audio, scripting, gameplay  
- **Input Manager**: Keyboard/mouse capture, customizable bindings, console overlay  

### 🛠 Editor & Scripting
- **Visual Editor** *(optional)*: ImGui-powered scene hierarchy, inspector, asset browser, gizmos  
- **AngelScript Integration**: Unity-style hot-reload, full debugging, exposed C++ API  

### 🩹 Stability & Debug
- **Crash Handler**: Auto minidumps, symbolized stack traces, screenshot capture, ZIP upload  
- **Console Overlay**: Real-time command input, logging, test crash trigger  

### ⚙️ Utilities
- **Shader System**: HLSL support with auto-recompilation and constant-buffer fixes  
- **Mesh Generation**: Procedural cube, plane, and sphere primitives  
- **Cross-Platform Build**: CMake for Windows, Linux, macOS; auto-discovers sources  

## 📋 Table of Contents

- [Introduction](#spark-engine)  
- [Quick Start](#quick-start)  
- [Build & Setup](#build--setup)  
- [Controls](#controls)  
- [Architecture Overview](#architecture-overview)  
- [Directory Layout](#directory-layout)  
- [Configuration Options](#configuration-options)  
- [License](#license)  

## 🔥 Quick Start

```bash
# Clone repo with submodules
git clone --recurse-submodules https://github.com/YourOrg/SparkEngine.git
cd SparkEngine

# Generate build files
# Windows (PowerShell)
.\generate.bat -g "Visual Studio 17 2022" release

# Linux/macOS
chmod +x generate.sh
./generate.sh release -g Ninja

# Build
# Windows
.\build.ps1 -config Release -editor -angelscript

# Linux/macOS
./build.sh release

# Run
# Windows
build/Release/SparkEngine.exe

# Linux/macOS
build/SparkEngine
```

## 🎮 Controls

| Input         | Action                            |
|---------------|-----------------------------------|
| W/A/S/D       | Move                              |
| Mouse         | Look                              |
| Space         | Jump / Move Up                    |
| Ctrl          | Crouch / Move Down                |
| Esc           | Release Mouse / Toggle Menu       |
| Left Click    | (Re)Capture Mouse                 |
| ` (Backtick)  | Toggle Debug Console              |

## 🏗 Architecture Overview

```
┌─────────────────┬─────────────────┬─────────────────┐
│  Rendering      │  Physics        │  Audio          │
├─────────────────┼─────────────────┼─────────────────┤
│ • GraphicsEngine│ • PhysicsSystem │ • AudioSystem   │
│ • ShaderManager │ • Collision     │ • Mixer         │
│ • MeshManager   │ • RigidBodies   │ • Effects       │
└─────────────────┴─────────────────┴─────────────────┘

┌─────────────────┬─────────────────┬─────────────────┐
│  Scripting      │  Input & UI     │  Core & Utils   │
├─────────────────┼─────────────────┼─────────────────┤
│ • AngelScript   │ • InputManager  │ • Engine        │
│ • ScriptEngine  │ • Console       │ • Timer         │
│ • Bindings      │ • ImGui Editor  │ • FileSystem    │
└─────────────────┴─────────────────┴─────────────────┘
```

## 📂 Directory Layout

```
.
├── Source/
│   ├── Core/            # Entry point & framework
│   ├── Graphics/        # DX11 renderer, shaders
│   ├── Game/            # Gameplay logic & objects
│   ├── Camera/          # FPS camera
│   ├── Input/           # Input handling & console
│   ├── Utils/           # Timer, logging, file I/O
│   └── Scripting/       # AngelScript integration
├── ThirdParty/          # Submodules (EnTT, ImGui, AngelScript…)
├── Shaders/             # HLSL files
├── Resources/           # Models, textures, sounds
├── generate.bat/.sh     # CMake configure-only scripts
├── build.ps1/.sh        # Full build scripts
├── CMakeLists.txt       # Cross-platform build config
└── README.md            # This file
```

## ⚙️ Configuration Options

| Option                 | Default | Description                                    |
|------------------------|---------|------------------------------------------------|
| ENABLE_EDITOR          | ON      | Include in-engine editor                       |
| ENABLE_CONSOLE         | ON      | External debug console overlay                 |
| ENABLE_ANGELSCRIPT     | ON      | AngelScript hot-reload scripting               |
| ENABLE_VULKAN          | OFF     | Build Vulkan renderer                          |
| USE_STATIC_RUNTIME     | ON      | Static CRT on MSVC (/MT)                       |

Pass these to `generate.bat/.sh` or edit `CMakeLists.txt` directly:

```bash
cmake .. -DENABLE_EDITOR=OFF -DENABLE_ANGELSCRIPT=OFF
```

## 📄 License

MIT License.  
Commercial support & enterprise add-ons available—visit our website for details.