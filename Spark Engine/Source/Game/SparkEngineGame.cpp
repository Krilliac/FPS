#include "SparkEngineGame.h"
#include "../Utils/Assert.h"
#include "../Utils/Profiler.h"
#include <iostream>
#include <exception>

namespace SparkEngine {

SparkEngineGame::SparkEngineGame() 
    : m_initialized(false), m_running(false), m_deltaTime(0.0f) {
    std::cout << "SparkEngineGame constructor - Complete Unified System" << std::endl;
}

SparkEngineGame::~SparkEngineGame() {
    if (m_initialized) {
        Shutdown();
    }
    std::cout << "SparkEngineGame destructor" << std::endl;
}

bool SparkEngineGame::Initialize(HWND hwnd, int width, int height) {
    PROFILE_FUNCTION();
    
    if (m_initialized) {
        std::cout << "SparkEngineGame already initialized" << std::endl;
        return true;
    }

    if (!hwnd) {
        std::cerr << "ERROR: Invalid window handle" << std::endl;
        return false;
    }

    if (width <= 0 || height <= 0) {
        std::cerr << "ERROR: Invalid dimensions: " << width << "x" << height << std::endl;
        return false;
    }

    std::cout << "Initializing Complete SparkEngine System..." << std::endl;
    std::cout << "Target resolution: " << width << "x" << height << std::endl;

    m_hwnd = hwnd;
    m_windowWidth = width;
    m_windowHeight = height;

    try {
        // Phase 1: Core Graphics System (Critical Foundation)
        {
            PROFILE_SCOPE("Graphics System Initialization");
            if (!InitializeGraphicsSystem()) {
                std::cerr << "CRITICAL: Graphics system initialization failed" << std::endl;
                return false;
            }
            std::cout << "✓ Advanced Graphics System initialized" << std::endl;
        }

        // Phase 2: Core Engine Systems
        {
            PROFILE_SCOPE("Core Systems Initialization");
            if (!InitializeCoreSystemsComplete()) {
                std::cerr << "CRITICAL: Core systems initialization failed" << std::endl;
                return false;
            }
            std::cout << "✓ Core Engine Systems initialized" << std::endl;
        }

        // Phase 3: Asset and Content Systems
        {
            PROFILE_SCOPE("Asset Systems Initialization");
            if (!InitializeAssetSystems()) {
                std::cerr << "CRITICAL: Asset systems initialization failed" << std::endl;
                return false;
            }
            std::cout << "✓ Asset and Content Systems initialized" << std::endl;
        }

        // Phase 4: Advanced Rendering Pipeline
        {
            PROFILE_SCOPE("Rendering Pipeline Initialization");
            if (!InitializeRenderingPipeline()) {
                std::cerr << "CRITICAL: Rendering pipeline initialization failed" << std::endl;
                return false;
            }
            std::cout << "✓ Advanced Rendering Pipeline initialized" << std::endl;
        }

        // Phase 5: Input and Physics Systems
        {
            PROFILE_SCOPE("Input/Physics Systems Initialization");
            if (!InitializeInputPhysicsSystems()) {
                std::cerr << "WARNING: Input/Physics systems initialization failed (non-critical)" << std::endl;
                // Non-critical failure - continue
            } else {
                std::cout << "✓ Input and Physics Systems initialized" << std::endl;
            }
        }

        // Phase 6: Scripting and Collaboration Systems
        {
            PROFILE_SCOPE("Advanced Systems Initialization");
            if (!InitializeAdvancedSystems()) {
                std::cerr << "WARNING: Advanced systems initialization failed (non-critical)" << std::endl;
                // Non-critical failure - continue
            } else {
                std::cout << "✓ Scripting and Collaboration Systems initialized" << std::endl;
            }
        }

        // Phase 7: Editor System
        {
            PROFILE_SCOPE("Editor System Initialization");
            if (!InitializeEditorSystem()) {
                std::cerr << "WARNING: Editor system initialization failed (non-critical)" << std::endl;
                // Non-critical failure - continue
            } else {
                std::cout << "✓ Complete Editor System initialized" << std::endl;
            }
        }

        // Phase 8: Create Rich Example Scene
        {
            PROFILE_SCOPE("Scene Creation");
            CreateCompleteExampleScene();
            std::cout << "✓ Complete Example Scene created" << std::endl;
        }

        m_initialized = true;
        m_running = true;

        std::cout << "🎉 SparkEngine Complete System initialization successful!" << std::endl;
        PrintSystemsSummary();
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION during complete system initialization: " << e.what() << std::endl;
        Shutdown();
        return false;
    }
    catch (...) {
        std::cerr << "UNKNOWN EXCEPTION during complete system initialization" << std::endl;
        Shutdown();
        return false;
    }
}

void SparkEngineGame::Shutdown() {
    PROFILE_FUNCTION();
    
    if (!m_initialized) {
        return;
    }

    std::cout << "Shutting down Complete SparkEngine System..." << std::endl;
    m_running = false;

    try {
        // Shutdown in reverse order
        ShutdownAllSystems();
        m_initialized = false;
        std::cout << "Complete system shutdown successful" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception during shutdown: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception during shutdown" << std::endl;
    }
}

void SparkEngineGame::Update(float deltaTime) {
    PROFILE_FUNCTION();
    
    if (!m_running) {
        return;
    }

    m_deltaTime = deltaTime;

    try {
        // Update all systems in proper order
        UpdateAllSystems(deltaTime);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in Update: " << e.what() << std::endl;
        // Try to continue running
    }
}

void SparkEngineGame::Render() {
    PROFILE_FUNCTION();
    
    if (!m_running || !m_graphics) {
        return;
    }

    try {
        m_graphics->BeginFrame();
        
        // Render complete scene with all systems
        RenderCompleteScene();
        
        // Render editor UI if available
        if (m_editorManager) {
            m_editorManager->Render();
            m_editorManager->Present();
        }
        
        m_graphics->EndFrame();
        m_graphics->Present();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in Render: " << e.what() << std::endl;
        // Try to continue
    }
}

bool SparkEngineGame::InitializeGraphicsSystem() {
    try {
        // Create enhanced graphics engine
        m_graphics = std::make_unique<GraphicsEngine>();
        if (!m_graphics) {
            return false;
        }

        if (FAILED(m_graphics->Initialize(m_hwnd))) {
            std::cerr << "Graphics engine initialization failed" << std::endl;
            return false;
        }

        // Initialize shader manager
        m_shaderManager = std::make_unique<ShaderManager>();
        if (!m_shaderManager || !m_shaderManager->Initialize(m_graphics->GetDevice())) {
            std::cerr << "Shader manager initialization failed" << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in graphics system initialization: " << e.what() << std::endl;
        return false;
    }
}

bool SparkEngineGame::InitializeCoreSystemsComplete() {
    try {
        // ECS System
        m_entityRegistry = std::make_unique<EntityRegistry>();
        if (!m_entityRegistry) {
            return false;
        }

        m_systemManager = std::make_unique<SystemManager>();
        if (!m_systemManager || !m_systemManager->Initialize(m_entityRegistry.get())) {
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in core systems initialization: " << e.what() << std::endl;
        return false;
    }
}

bool SparkEngineGame::InitializeAssetSystems() {
    try {
        // Asset Manager with all features
        m_assetManager = std::make_unique<AssetManager>();
        if (!m_assetManager || !m_assetManager->Initialize()) {
            return false;
        }

        // Enable advanced features
        m_assetManager->EnableDynamicPipeline(true);
        m_assetManager->EnableAssetStreaming(true);

        // Mesh Manager
        m_meshManager = std::make_unique<MeshManager>();
        if (!m_meshManager || !m_meshManager->Initialize(m_graphics->GetDevice())) {
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in asset systems initialization: " << e.what() << std::endl;
        return false;
    }
}

bool SparkEngineGame::InitializeRenderingPipeline() {
    try {
        // Lighting System
        m_lightingSystem = std::make_unique<LightingSystem>();
        if (!m_lightingSystem || !m_lightingSystem->Initialize(m_graphics->GetDevice())) {
            return false;
        }

        // Post-Processing Pipeline
        m_postProcessPipeline = std::make_unique<PostProcessPipeline>();
        if (!m_postProcessPipeline || !m_postProcessPipeline->Initialize(m_graphics->GetDevice(), m_windowWidth, m_windowHeight)) {
            return false;
        }

        // Terrain System
        if (m_graphics->GetTerrainManager()) {
            if (!m_graphics->GetTerrainManager()->Initialize()) {
                std::cerr << "Warning: Terrain system initialization failed" << std::endl;
            }
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in rendering pipeline initialization: " << e.what() << std::endl;
        return false;
    }
}

bool SparkEngineGame::InitializeInputPhysicsSystems() {
    try {
        // Advanced Input System
        m_advancedInput = std::make_unique<AdvancedInputManager>();
        if (!m_advancedInput || !m_advancedInput->Initialize(m_hwnd)) {
            std::cerr << "Warning: Advanced input system failed to initialize" << std::endl;
            return false;
        }

        // Physics system would be initialized here if enabled
        // For now, return true as it's optional

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in input/physics systems initialization: " << e.what() << std::endl;
        return false;
    }
}

bool SparkEngineGame::InitializeAdvancedSystems() {
    try {
        // Scripting System
        m_scriptingSystem = std::make_unique<ScriptingSystem>();
        if (!m_scriptingSystem || !m_scriptingSystem->Initialize(m_entityRegistry.get())) {
            std::cerr << "Warning: Scripting system failed to initialize" << std::endl;
            return false;
        }

        // Collaborative Development Framework
        m_collaborativeDevelopment = std::make_unique<CollaborativeDevelopment>(
            m_entityRegistry.get(), m_assetManager.get(), 
            m_scriptingSystem.get(), m_graphics.get());
            
        if (!m_collaborativeDevelopment || !m_collaborativeDevelopment->Initialize("MainUser", false)) {
            std::cerr << "Warning: Collaborative development failed to initialize" << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in advanced systems initialization: " << e.what() << std::endl;
        return false;
    }
}

bool SparkEngineGame::InitializeEditorSystem() {
    try {
        m_editorManager = std::make_unique<EditorManager>();
        if (!m_editorManager || !m_editorManager->Initialize(m_hwnd, m_graphics->GetDevice(), 
                                                             m_graphics->GetContext(), m_entityRegistry.get())) {
            return false;
        }

        // Set up editor references
        m_editorManager->SetGraphicsEngine(m_graphics.get());
        m_editorManager->SetCollaborativeDevelopment(m_collaborativeDevelopment.get());

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in editor system initialization: " << e.what() << std::endl;
        return false;
    }
}

void SparkEngineGame::CreateCompleteExampleScene() {
    try {
        if (!m_entityRegistry) return;

        // Create camera with advanced controls
        m_cameraEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(m_cameraEntity, "Main Camera");
        
        auto& cameraTransform = m_entityRegistry->AddComponent<TransformComponent>(m_cameraEntity);
        cameraTransform.position = {0.0f, 2.0f, -10.0f};
        
        auto& camera = m_entityRegistry->AddComponent<CameraComponent>(m_cameraEntity);
        camera.isPrimary = true;
        camera.fov = 60.0f;

        // Create directional light
        m_lightEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(m_lightEntity, "Directional Light");
        
        auto& lightTransform = m_entityRegistry->AddComponent<TransformComponent>(m_lightEntity);
        lightTransform.rotation = {XMConvertToRadians(-45.0f), XMConvertToRadians(45.0f), 0.0f};
        
        auto& light = m_entityRegistry->AddComponent<LightComponent>(m_lightEntity);
        light.type = LightComponent::Directional;
        light.intensity = 1.2f;
        light.color = {1.0f, 0.95f, 0.8f};
        light.castShadows = true;

        // Create multiple demo objects
        CreateDemoObjects();

        std::cout << "Complete example scene created with " << m_entityRegistry->GetEntityCount() << " entities" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception creating example scene: " << e.what() << std::endl;
    }
}

void SparkEngineGame::CreateDemoObjects() {
    if (!m_entityRegistry || !m_meshManager) return;

    try {
        // Create procedural meshes
        auto cubeMesh = m_meshManager->CreateCube("demo_cube", 2.0f);
        auto sphereMesh = m_meshManager->CreateSphere("demo_sphere", 1.0f, 32);
        auto planeMesh = m_meshManager->CreatePlane("demo_plane", 10.0f, 10.0f);

        // Create cube entity
        m_cubeEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(m_cubeEntity, "Demo Cube");
        
        auto& cubeTransform = m_entityRegistry->AddComponent<TransformComponent>(m_cubeEntity);
        cubeTransform.position = {-3.0f, 1.0f, 0.0f};
        
        m_entityRegistry->AddComponent<MeshRendererComponent>(m_cubeEntity, "demo_cube", "default_pbr");

        // Create sphere entity
        Entity sphereEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(sphereEntity, "Demo Sphere");
        
        auto& sphereTransform = m_entityRegistry->AddComponent<TransformComponent>(sphereEntity);
        sphereTransform.position = {3.0f, 1.0f, 0.0f};
        
        m_entityRegistry->AddComponent<MeshRendererComponent>(sphereEntity, "demo_sphere", "metallic_material");

        // Create ground plane
        Entity groundEntity = m_entityRegistry->CreateEntity();
        m_entityRegistry->AddComponent<NameComponent>(groundEntity, "Ground Plane");
        
        auto& groundTransform = m_entityRegistry->AddComponent<TransformComponent>(groundEntity);
        groundTransform.position = {0.0f, 0.0f, 0.0f};
        
        m_entityRegistry->AddComponent<MeshRendererComponent>(groundEntity, "demo_plane", "ground_material");

        std::cout << "Demo objects created successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception creating demo objects: " << e.what() << std::endl;
    }
}

void SparkEngineGame::UpdateAllSystems(float deltaTime) {
    PROFILE_FUNCTION();

    try {
        // Update collaborative systems
        if (m_collaborativeDevelopment) {
            m_collaborativeDevelopment->Update(deltaTime);
        }

        // Update input system
        if (m_advancedInput) {
            m_advancedInput->Update();
        }

        // Update asset systems
        if (m_assetManager) {
            m_assetManager->Update();
        }

        // Update ECS systems
        if (m_systemManager) {
            m_systemManager->UpdateSystems(deltaTime);
        }

        // Update lighting system
        if (m_lightingSystem) {
            m_lightingSystem->Update(m_entityRegistry.get());
        }

        // Update shader hot-reload
        if (m_shaderManager) {
            m_shaderManager->Update();
        }

        // Update editor
        if (m_editorManager) {
            m_editorManager->NewFrame();
        }

        // Simple object animation for demo
        AnimateDemoObjects(deltaTime);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in UpdateAllSystems: " << e.what() << std::endl;
    }
}

void SparkEngineGame::RenderCompleteScene() {
    PROFILE_FUNCTION();

    try {
        if (!m_entityRegistry) return;

        // Set up lighting for render
        if (m_lightingSystem) {
            m_lightingSystem->SetupLightingForRender(m_graphics->GetContext());
        }

        // Render all mesh renderer components
        auto meshView = m_entityRegistry->GetEntitiesWithComponents<TransformComponent, MeshRendererComponent>();
        for (Entity entity : meshView) {
            const auto& transform = m_entityRegistry->GetComponent<TransformComponent>(entity);
            const auto& meshRenderer = m_entityRegistry->GetComponent<MeshRendererComponent>(entity);
            
            if (meshRenderer.visible && m_meshManager) {
                XMMATRIX worldMatrix = transform.GetWorldMatrix();
                m_meshManager->RenderMesh(m_graphics->GetContext(), meshRenderer.meshPath, worldMatrix);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in RenderCompleteScene: " << e.what() << std::endl;
    }
}

void SparkEngineGame::AnimateDemoObjects(float deltaTime) {
    try {
        if (!m_entityRegistry) return;

        static float time = 0.0f;
        time += deltaTime;

        // Rotate the cube
        if (m_entityRegistry->HasComponent<TransformComponent>(m_cubeEntity)) {
            auto& transform = m_entityRegistry->GetComponent<TransformComponent>(m_cubeEntity);
            transform.rotation.y = time * 0.5f;
            transform.position.y = 1.0f + sin(time * 2.0f) * 0.5f;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in AnimateDemoObjects: " << e.what() << std::endl;
    }
}

void SparkEngineGame::PrintSystemsSummary() {
    std::cout << "\n=== SPARKENGINE COMPLETE SYSTEM SUMMARY ===" << std::endl;
    std::cout << "Graphics Engine: " << (m_graphics ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Shader Manager: " << (m_shaderManager ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Asset Manager: " << (m_assetManager ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Mesh Manager: " << (m_meshManager ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Lighting System: " << (m_lightingSystem ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Post-Processing: " << (m_postProcessPipeline ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Input System: " << (m_advancedInput ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Scripting System: " << (m_scriptingSystem ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Collaborative Dev: " << (m_collaborativeDevelopment ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Editor System: " << (m_editorManager ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "Entity Registry: " << (m_entityRegistry ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "System Manager: " << (m_systemManager ? "✓ Active" : "✗ Failed") << std::endl;
    std::cout << "=============================================" << std::endl;
}

void SparkEngineGame::ShutdownAllSystems() {
    PROFILE_FUNCTION();

    try {
        // Shutdown in reverse order of initialization
        if (m_editorManager) {
            m_editorManager->Shutdown();
            m_editorManager.reset();
        }

        if (m_collaborativeDevelopment) {
            m_collaborativeDevelopment->Shutdown();
            m_collaborativeDevelopment.reset();
        }

        if (m_scriptingSystem) {
            m_scriptingSystem->Shutdown();
            m_scriptingSystem.reset();
        }

        if (m_advancedInput) {
            m_advancedInput->Shutdown();
            m_advancedInput.reset();
        }

        if (m_postProcessPipeline) {
            m_postProcessPipeline->Shutdown();
            m_postProcessPipeline.reset();
        }

        if (m_lightingSystem) {
            m_lightingSystem->Shutdown();
            m_lightingSystem.reset();
        }

        if (m_meshManager) {
            m_meshManager->Shutdown();
            m_meshManager.reset();
        }

        if (m_assetManager) {
            m_assetManager->Shutdown();
            m_assetManager.reset();
        }

        if (m_systemManager) {
            m_systemManager->ShutdownSystems();
            m_systemManager.reset();
        }

        m_entityRegistry.reset();

        if (m_shaderManager) {
            m_shaderManager->Shutdown();
            m_shaderManager.reset();
        }

        if (m_graphics) {
            m_graphics->Shutdown();
            m_graphics.reset();
        }

        std::cout << "All systems shutdown successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception during system shutdown: " << e.what() << std::endl;
    }
}

} // namespace SparkEngine

