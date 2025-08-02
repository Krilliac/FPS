#include "Engine.h"
#include "../Utils/Timer.h"
#include "../Utils/Logger.h"

namespace SparkEngine {
    Engine* Engine::s_instance = nullptr;

    Engine::Engine() 
        : m_running(false), m_initialized(false) {
        s_instance = this;
        
        // Initialize subsystems
        m_graphics = std::make_unique<GraphicsEngine>();
        m_audio = std::make_unique<AudioSystem>();
        m_input = std::make_unique<InputSystem>();
        m_registry = std::make_unique<EntityRegistry>();
        m_assets = std::make_unique<AssetManager>();

#ifdef PHYSX_AVAILABLE
        m_physics = std::make_unique<PhysicsSystem>();
#endif

#ifdef LUA_SCRIPTING_ENABLED
        m_scripting = std::make_unique<ScriptingSystem>();
#endif

#ifdef EDITOR_ENABLED
        m_editor = std::make_unique<EditorSystem>();
#endif
    }

    Engine::~Engine() {
        Shutdown();
        s_instance = nullptr;
    }

    bool Engine::Initialize() {
        Logger::Info("Initializing SparkEngine...");

        // Initialize graphics first
        if (!m_graphics->Initialize()) {
            Logger::Error("Failed to initialize graphics engine");
            return false;
        }

        // Initialize audio
        if (!m_audio->Initialize()) {
            Logger::Warning("Failed to initialize audio system");
        }

        // Initialize assets
        if (!m_assets->Initialize()) {
            Logger::Error("Failed to initialize asset manager");
            return false;
        }

#ifdef PHYSX_AVAILABLE
        if (!m_physics->Initialize()) {
            Logger::Warning("Failed to initialize physics system");
        }
#endif

#ifdef LUA_SCRIPTING_ENABLED
        if (!m_scripting->Initialize()) {
            Logger::Warning("Failed to initialize scripting system");
        }
#endif

#ifdef EDITOR_ENABLED
        if (!m_editor->Initialize()) {
            Logger::Warning("Failed to initialize editor system");
        }
#endif

        m_initialized = true;
        Logger::Info("SparkEngine initialized successfully");
        return true;
    }

    void Engine::Run() {
        if (!m_initialized) {
            Logger::Error("Engine not initialized!");
            return;
        }

        m_running = true;
        Timer timer;

        while (m_running) {
            float deltaTime = timer.GetDeltaTime();
            
            // Update input first
            m_input->Update();
            
            // Check for exit conditions
            if (m_input->IsKeyPressed(VK_ESCAPE)) {
                m_running = false;
            }

            Update(deltaTime);
            Render();
        }
    }

    void Engine::Update(float deltaTime) {
        // Update registry
        // m_registry->Update(deltaTime);

#ifdef PHYSX_AVAILABLE
        m_physics->Update(deltaTime);
#endif

#ifdef LUA_SCRIPTING_ENABLED
        m_scripting->Update(deltaTime);
#endif

#ifdef EDITOR_ENABLED
        m_editor->Update(deltaTime);
#endif

        m_audio->Update();
    }

    void Engine::Render() {
        m_graphics->BeginFrame();

#ifdef EDITOR_ENABLED
        m_editor->Render();
#endif

        m_graphics->EndFrame();
    }

    void Engine::Shutdown() {
        if (!m_initialized) return;

        Logger::Info("Shutting down SparkEngine...");

#ifdef EDITOR_ENABLED
        m_editor->Shutdown();
#endif

#ifdef LUA_SCRIPTING_ENABLED
        m_scripting->Shutdown();
#endif

#ifdef PHYSX_AVAILABLE
        m_physics->Shutdown();
#endif

        m_assets->Shutdown();
        m_audio->Shutdown();
        m_graphics->Shutdown();

        m_initialized = false;
        Logger::Info("SparkEngine shutdown complete");
    }
}
