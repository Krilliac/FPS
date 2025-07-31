#include <imgui.h>
#include "Utils/FileWatcher.h"
#include "Utils/Assert.h"
#include "Utils/CrashHandler.h"
#include "Assets/AssetSystem.h"
#include "Editor/EditorSystem.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Scripting/ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/Systems/RenderSystem.h"
#include "Game/SparkEngineGame.h"
#include "D3DUtils.h"

// Define the globals
IDXGISwapChain* g_MainSwapChain = nullptr;
ID3D11Device* g_D3DDevice = nullptr;
ID3D11DeviceContext* g_D3DContext = nullptr;

// Implement the accessors
IDXGISwapChain* GetMainSwapChain() { return g_MainSwapChain; }
ID3D11Device* GetD3DDevice() { return g_D3DDevice; }
ID3D11DeviceContext* GetD3DContext() { return g_D3DContext; }