#pragma once
#include "../Core/framework.h"
#include "../ECS/EntityRegistry.h"
#include "../ECS/System.h"
#include "../ECS/Components.h"
#include "../Scripting/ScriptingSystem.h"
#include "../Graphics/Materials/PBRMaterial.h"
#include "../Assets/AssetManager.h"
#include "../Editor/EditorManager.h"

#ifdef SPARKENGINE_PHYSICS_ENABLED
#include "../Physics/PhysicsSystem.h"
#endif

namespace SparkEngine {
    class SparkEngineGame {
    private:
        // Core systems
        std::unique_ptr<EntityRegistry> m_entityRegistry;
        std::unique_ptr<SystemManager> m_systemManager;
        std::unique_ptr<ScriptingSystem> m_scriptingSystem;
        std::unique_ptr<AssetManager> m_assetManager;
        std::unique_ptr<EditorManager> m_editorManager;

        // Graphics
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;
        ComPtr<ID3D11SwapChain> m_swapChain;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        // Game state
        bool m_initialized = false;
        bool m_running = false;
        float m_deltaTime = 0.0f;

        // Example entities
        Entity m_cameraEntity = entt::null;
        Entity m_lightEntity = entt::null;
        Entity m_cubeEntity = entt::null;

    public:
        SparkEngineGame() = default;
        ~SparkEngineGame() = default;

        bool Initialize(HWND hwnd, int width, int height);
        void Shutdown();

        void Update(float deltaTime);
        void Render();

        bool IsRunning() const { return m_running; }
        void Stop() { m_running = false; }

        // System accessors
        EntityRegistry* GetEntityRegistry() { return m_entityRegistry.get(); }
        SystemManager* GetSystemManager() { return m_systemManager.get(); }
        ScriptingSystem* GetScriptingSystem() { return m_scriptingSystem.get(); }
        AssetManager* GetAssetManager() { return m_assetManager.get(); }
        EditorManager* GetEditorManager() { return m_editorManager.get(); }

    private:
        bool InitializeD3D11(HWND hwnd, int width, int height);
        bool InitializeSystems();
        void CreateExampleScene();
        void UpdateSystems(float deltaTime);
        void RenderScene();
    };
}