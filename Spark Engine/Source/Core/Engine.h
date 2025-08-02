#pragma once
#include "framework.h"
#include "../Graphics/GraphicsEngine.h"
#include "../Audio/AudioSystem.h"
#include "../Input/InputSystem.h"
#include "../ECS/EntityRegistry.h"
#include "../Assets/AssetManager.h"
#include "../Physics/PhysicsSystem.h"
#include "../Scripting/ScriptingSystem.h"
#include "../Editor/EditorSystem.h"

namespace SparkEngine {
    class Engine {
    public:
        Engine();
        ~Engine();

        bool Initialize();
        void Run();
        void Shutdown();

        // Subsystem access
        GraphicsEngine& GetGraphics() { return *m_graphics; }
        AudioSystem& GetAudio() { return *m_audio; }
        InputSystem& GetInput() { return *m_input; }
        EntityRegistry& GetRegistry() { return *m_registry; }
        AssetManager& GetAssets() { return *m_assets; }
        
#ifdef PHYSX_AVAILABLE
        PhysicsSystem& GetPhysics() { return *m_physics; }
#endif

#ifdef LUA_SCRIPTING_ENABLED
        ScriptingSystem& GetScripting() { return *m_scripting; }
#endif

#ifdef EDITOR_ENABLED
        EditorSystem& GetEditor() { return *m_editor; }
#endif

        static Engine& GetInstance() { return *s_instance; }

    private:
        void Update(float deltaTime);
        void Render();

        // Subsystems
        std::unique_ptr<GraphicsEngine> m_graphics;
        std::unique_ptr<AudioSystem> m_audio;
        std::unique_ptr<InputSystem> m_input;
        std::unique_ptr<EntityRegistry> m_registry;
        std::unique_ptr<AssetManager> m_assets;

#ifdef PHYSX_AVAILABLE
        std::unique_ptr<PhysicsSystem> m_physics;
#endif

#ifdef LUA_SCRIPTING_ENABLED
        std::unique_ptr<ScriptingSystem> m_scripting;
#endif

#ifdef EDITOR_ENABLED
        std::unique_ptr<EditorSystem> m_editor;
#endif

        // Core state
        bool m_running;
        bool m_initialized;

        static Engine* s_instance;
    };
}
