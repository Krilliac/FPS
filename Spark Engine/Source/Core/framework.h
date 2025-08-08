﻿/**
 * @file framework.h
 * @brief Core framework header with essential includes and library links
 * @author Spark Engine Team
 * @date 2024
 * 
 * This header provides the fundamental includes required throughout the engine,
 * including Windows API, DirectX 11, STL containers, and necessary library links.
 * It serves as a precompiled header equivalent for the engine's core dependencies.
 */

#pragma once

#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>  // For GET_X_LPARAM and GET_Y_LPARAM macros
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// DirectX includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

// STL includes
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <queue>

// Link libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

using namespace DirectX;
