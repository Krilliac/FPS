#pragma once

#include "../Core/framework.h"
#include "../ECS/EntityRegistry.h"
#include "../ECS/System.h"
#include "../ECS/Components.h"
#include "../Graphics/GraphicsEngine.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Graphics/PostProcessing/PostProcessPipeline.h"
#include "../Assets/AssetManager.h"
#include "../Input/AdvancedInputManager.h"
#include "../Scripting/ScriptingSystem.h"
#include "../Editor/EditorManager.h"
#include "../Development/CollaborativeDevelopment.h"
#include <memory>

namespace SparkEngine {
    class SparkEngineGame {
    public:
        SparkEngineGame();
        ~SparkEngineGame();

        // Core lifecycle
        bool Initialize(HWND hwnd, int width, int height);
        void Shutdown();
        void Update(float deltaTime);
        void Render();

        // State queries
        bool IsInitialized() const { return m_initialized; }
        bool IsRunning() const { return m_running; }

        // Window properties
        int GetWindowWidth() const { return m_windowWidth; }
        int GetWindowHeight() const { return m_windowHeight; }

        // System accessors
        GraphicsEngine* GetGraphicsEngine() const { return m_graphics.get(); }
        EntityRegistry* GetEntityRegistry() const { return m_entityRegistry.get(); }
        AssetManager* GetAssetManager() const { return m_assetManager.get(); }
        EditorManager* GetEditorManager() const { return m_editorManager.get(); }

    private:
        // Initialization phases
        bool InitializeGraphicsSystem();
        bool InitializeCoreSystemsComplete();
        bool InitializeAssetSystems();
        bool InitializeRenderingPipeline();
        bool InitializeInputPhysicsSystems();
        bool InitializeAdvancedSystems();
        bool InitializeEditorSystem();

        // Scene creation
        void CreateCompleteExampleScene();
        void CreateDemoObjects();

        // Update and render helpers
        void UpdateAllSystems(float deltaTime);
        void RenderCompleteScene();
        void AnimateDemoObjects(float deltaTime);

        // Utility
        void PrintSystemsSummary();
        void ShutdownAllSystems();

        // Core systems
        std::unique_ptr<GraphicsEngine> m_graphics;
        std::unique_ptr<EntityRegistry> m_entityRegistry;
        std::unique_ptr<SystemManager> m_systemManager;
        std::unique_ptr<AssetManager> m_assetManager;
        std::unique_ptr<EditorManager> m_editorManager;

        // Advanced graphics systems
        std::unique_ptr<ShaderManager> m_shaderManager;
        std::unique_ptr<MeshManager> m_meshManager;
        std::unique_ptr<LightingSystem> m_lightingSystem;
        std::unique_ptr<PostProcessPipeline> m_postProcessPipeline;

        // Input and interaction systems
        std::unique_ptr<AdvancedInputManager> m_advancedInput;
        std::unique_ptr<ScriptingSystem> m_scriptingSystem;

        // Collaborative development
        std::unique_ptr<CollaborativeDevelopment> m_collaborativeDevelopment;

        // Window properties
        HWND m_hwnd = nullptr;
        int m_windowWidth = 0;
        int m_windowHeight = 0;

        // Scene entities
        Entity m_cameraEntity = entt::null;
        Entity m_lightEntity = entt::null;
        Entity m_cubeEntity = entt::null;

        // State
        bool m_initialized = false;
        bool m_running = false;
        float m_deltaTime = 0.0f;
    };
}
