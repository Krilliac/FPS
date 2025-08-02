#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <wrl/client.h>

// DirectXMath
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

// STL
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <future>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <cassert>

// DirectX namespace and ComPtr<using> namespace DirectX;
// ComPtr alias removed; defined in framework.h

// Typedefs
typedef XMFLOAT2 Float2;
typedef XMFLOAT3 Float3;
typedef XMFLOAT4 Float4;
typedef XMMATRIX Matrix;
typedef XMVECTOR Vector;

// EnTT forward declare
#ifdef ENTT_AVAILABLE
namespace entt { enum class entity : std::uint32_t {}; template<typename E> class basic_registry; }
#endif

namespace SparkEngine {
    #ifdef ENTT_AVAILABLE
    using Entity = entt::entity;
    #else
    using Entity = std::uint32_t;
    #endif
    class EntityRegistry;
    class AssetManager;
    class GraphicsEngine;
}

