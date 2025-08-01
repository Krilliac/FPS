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
#include "SparkEngineGame.h"
#include <iostream>

namespace SparkEngine {
    bool SparkEngineGame::Initialize(HWND hwnd, int width, int height) {
        if (m_initialized) {
            return true;
        }

        std::cout << "Initializing SparkEngine..." << std::endl;

        // Initialize DirectX 11
        if (!InitializeD3D11(hwnd, width, height)) {
            std::cerr << "Failed to initialize DirectX 11" << std::endl;
            return false;
        }

        // Initialize all systems
        if (!InitializeSystems()) {
            std::cerr << "Failed to initialize game systems" << std::endl;
            return false;
        }

        // Create example scene
        
    // Initialize Collaborative Development Framework
    m_collaborativeDevelopment = std::make_unique<CollaborativeDevelopment>(
        m_entityRegistry.get(), m_assetManager.get(), 
        m_scriptingSystem.get(), m_graphics.get());
    
    if (!m_collaborativeDevelopment->Initialize("DefaultDeveloper", false)) {
        std::cerr << "Failed to initialize collaborative development framework" << std::endl;
    }

        m_initialized = true;
    // Initialize multi-threaded rendering
    RenderThreadManager* rtManager = new RenderThreadManager();
    rtManager->Initialize(m_device.Get());
    // Optional: store rtManager in game class for cleanup
        m_running = true;

        std::cout << "SparkEngine initialized successfully!" << std::endl;
        return true;
    }

    void SparkEngineGame::Shutdown() {
        // Shutdown collaborative systems
        if (m_collaborativeDevelopment) {
            m_collaborativeDevelopment->Shutdown();
        }
        if (!m_initialized) {
            return;
        }

        std::cout << "Shutting down SparkEngine..." << std::endl;

        // Shutdown systems in reverse order
        if (m_editorManager) {
            m_editorManager->Shutdown();
        }

        if (m_systemManager) {
            m_systemManager->ShutdownSystems();
        }

        if (m_scriptingSystem) {
            m_scriptingSystem->Shutdown();
        }

        if (m_assetManager) {
            m_assetManager->Shutdown();
        }

        // Clear DirectX resources
        m_depthStencilView.Reset();
        m_renderTargetView.Reset();
        m_swapChain.Reset();
        m_context.Reset();
        m_device.Reset();

        m_initialized = false;
        m_running = false;

        std::cout << "SparkEngine shutdown complete." << std::endl;
    }

    void SparkEngineGame::Update(float deltaTime) {
        // Update collaborative systems
        if (m_collaborativeDevelopment) {
            m_collaborativeDevelopment->Update(deltaTime);
        }
        if (!m_running) {
            return;
        }

        m_deltaTime = deltaTime;

        // Update editor first
        if (m_editorManager) {
            m_editorManager->NewFrame();
        }

        // Update all systems
        UpdateSystems(deltaTime);
    }

    void SparkEngineGame::Render() {
        if (!m_running || !m_context) {
            return;
        }

        // Clear render targets
        const float clearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), 
                                        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
                                        1.0f, 0);

        // Set render targets
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

        // Render scene
        RenderScene();

        // Render editor UI
        if (m_editorManager) {
            m_editorManager->Render();
            m_editorManager->Present();
        }

        // Present
        m_swapChain->Present(1, 0);
    }

    bool SparkEngineGame::InitializeD3D11(HWND hwnd, int width, int height) {
        // Create device and swap chain
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = 2;
        swapChainDesc.BufferDesc.Width = width;
        swapChainDesc.BufferDesc.Height = height;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        D3D_FEATURE_LEVEL featureLevel;
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &m_swapChain,
            &m_device,
            &featureLevel,
            &m_context
        );

        if (FAILED(hr)) {
            return false;
        }

        // Create render target view
        ComPtr<ID3D11Texture2D> backBuffer;
        hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        if (FAILED(hr)) {
            return false;
        }

        hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
        if (FAILED(hr)) {
            return false;
        }

        // Create depth stencil buffer
        ComPtr<ID3D11Texture2D> depthStencilBuffer;
        D3D11_TEXTURE2D_DESC depthBufferDesc = {};
        depthBufferDesc.Width = width;
        depthBufferDesc.Height = height;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
        if (FAILED(hr)) {
            return false;
        }

        hr = m_device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &m_depthStencilView);
        if (FAILED(hr)) {
            return false;
        }

        // Set viewport
        D3D11_VIEWPORT viewport = {};
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        m_context->RSSetViewports(1, &viewport);

        return true;
    }

    bool SparkEngineGame::InitializeSystems() {
        // Initialize collaborative development framework first
        m_collaborativeDevelopment = std::make_unique<CollaborativeDevelopment>(
            m_entityRegistry.get(), m_assetManager.get(), 
            m_scriptingSystem.get(), m_graphics.get());
        
        if (!m_collaborativeDevelopment->Initialize("SparkEngineUser", false)) {
            std::cerr << "Warning: Failed to initialize collaborative development framework" << std::endl;
        }
        // Create core systems
        m_entityRegistry = std::make_unique<EntityRegistry>();
        m_systemManager = std::make_unique<SystemManager>();
        m_scriptingSystem = std::make_unique<ScriptingSystem>();
        m_assetManager = std::make_unique<AssetManager>();
        m_editorManager = std::make_unique<EditorManager>();

        // Initialize systems
        m_systemManager->Initialize(m_entityRegistry.get());

        if (!m_scriptingSystem->Initialize(m_entityRegistry.get())) {
            return false;
        }

        if (!m_assetManager->Initialize()) {
            return false;
        }

        // Register asset types
        m_assetManager->RegisterAssetType<TextureAsset>();
        m_assetManager->RegisterAssetType<MeshAsset>();

        if (!m_editorManager->Initialize(GetActiveWindow(), m_device.Get(), m_context.Get(), m_entityRegistry.get())) {
            return false;
        }

        // Add editor windows
        m_editorManager->AddWindow<HierarchyWindow>(m_entityRegistry.get(), 
            [this](Entity entity) { m_editorManager->SelectEntity(entity); });
        m_editorManager->AddWindow<InspectorWindow>(m_entityRegistry.get());

#ifdef SPARKENGINE_PHYSICS_ENABLED
        // Add physics system
        auto* physicsSystem = m_systemManager->AddSystem<PhysicsSystem>();
        if (!physicsSystem) {
            std::cerr << "Failed to add physics system" << std::endl;
        }
#endif

        return true;
    }

    void SparkEngineGame::CreateExampleScene() {
        // Create camera entity
        m_cameraEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(m_cameraEntity, "Main Camera");

        auto& cameraTransform = m_entityRegistry->AddComponent<TransformComponent>(m_cameraEntity);
        cameraTransform.position = {0.0f, 0.0f, -5.0f};

        auto& camera = m_entityRegistry->AddComponent<CameraComponent>(m_cameraEntity);
        camera.isPrimary = true;

        // Create light entity
        m_lightEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(m_lightEntity, "Directional Light");

        auto& lightTransform = m_entityRegistry->AddComponent<TransformComponent>(m_lightEntity);
        lightTransform.rotation = {DirectX::XMConvertToRadians(45.0f), DirectX::XMConvertToRadians(45.0f), 0.0f};

        auto& light = m_entityRegistry->AddComponent<LightComponent>(m_lightEntity);
        light.type = LightComponent::Directional;
        light.intensity = 1.0f;

        // Create cube entity
        m_cubeEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(m_cubeEntity, "Cube");
        m_entityRegistry->AddComponent<TransformComponent>(m_cubeEntity);
        m_entityRegistry->AddComponent<MeshRendererComponent>(m_cubeEntity, "cube.obj", "default.mat");

        std::cout << "Example scene created with " << m_entityRegistry->GetEntityCount() << " entities." << std::endl;
    }

    void SparkEngineGame::UpdateSystems(float deltaTime) {
        // Update collaborative systems
        if (m_collaborativeDevelopment) {
            m_collaborativeDevelopment->Update(deltaTime);
        }
        // Update all registered systems
        m_systemManager->UpdateSystems(deltaTime);
    }

    void SparkEngineGame::RenderScene() {
		// Clear render targets (already done in Render())
		// Example: Draw a simple cube
		if (auto cube = m_entityRegistry->GetComponent<MeshRendererComponent>(m_cubeEntity)) {
			// Assume GraphicsEngine::DrawMesh takes meshPath & transform
			DirectX::XMMATRIX world = m_entityRegistry->GetComponent<TransformComponent>(m_cubeEntity).GetWorldMatrix();
			g_graphics->DrawMesh(cube->meshPath, world);
		}
    }
}



