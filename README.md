# 3D FPS Game

A first-person shooter game built with DirectX 11 and the Windows API.

## Features

- 3D graphics rendering with DirectX 11
- First-person camera with mouse look
- Input handling for keyboard and mouse
- Organized modular architecture
- Real-time game loop with proper timing
- Mesh system with cube, plane, and sphere generation
- Shader system with HLSL support
- Object-oriented game architecture

## Building

1. Open FPS.sln in Visual Studio 2019 or later
2. Ensure Windows SDK is installed
3. Build in Debug or Release configuration

## Controls

- WASD: Movement
- Mouse: Look around
- Space: Jump/Move up
- Ctrl: Crouch/Move down
- Escape: Release mouse cursor
- Left Click: Capture mouse for FPS controls

## Project Structure

- Source/Core/: Main application and framework
- Source/Graphics/: DirectX 11 rendering engine
- Source/Game/: Game logic and objects
- Source/Camera/: FPS camera system
- Source/Input/: Input handling
- Source/Utils/: Utility classes
- Shaders/: HLSL shader files
- Resources/: Game assets and configuration

## Requirements

- Windows 10 or later
- DirectX 11 compatible graphics card
- Visual Studio 2019 or later with C++ support
