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
#include "ScriptingSystem.h"
#include "../ECS/Components.h"
#include <fstream>
#include <iostream>

namespace SparkEngine {
    bool ScriptingSystem::Initialize
    // bind update hook
    m_scripts["OnUpdate"] = m_lua.globals()["OnUpdate"];(EntityRegistry* registry) {
        m_registry = registry;

        // Initialize Lua state
        m_lua.open_libraries(sol::lib::base,
                            sol::lib::package,
                            sol::lib::coroutine,
                            sol::lib::string,
                            sol::lib::os,
                            sol::lib::math,
                            sol::lib::table,
                            sol::lib::debug,
                            sol::lib::bit32,
                            sol::lib::io,
                            sol::lib::utf8);

        // Register engine APIs
        RegisterEngineAPI();
    // Expose core systems to Lua
    m_lua["CreateEntity"] = [this]() { return m_registry->CreateEntity(); };
    m_lua["DestroyEntity"] = [this](Entity e) { m_registry->DestroyEntity(e); };
    m_lua["GetAsset"] = [this](const std::string& path) {
        return m_assetManager->GetAsset(path);
    };RegisterEntityAPI();
        RegisterComponentAPI();

        return true;
    }

    void ScriptingSystem::Shutdown() {
        m_scripts.clear();
        // Lua state destructor handles cleanup
    }

    bool ScriptingSystem::LoadScript(const std::string& filepath, const std::string& name) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return false;
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());

        try {
            sol::protected_function_result result = m_lua.safe_script(content, sol::script_pass_on_error);
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "Lua Script Error in " << filepath << ": " << err.what() << std::endl;
                return false;
            }

            // Store the script environment
            m_scripts[name] = m_lua.globals();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Exception loading script " << filepath << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool ScriptingSystem::ExecuteScript(const std::string& name) {
        auto it = m_scripts.find(name);
        if (it != m_scripts.end()) {
            return CallFunction(name, "main");
        }
        return false;
    }

    bool ScriptingSystem::ReloadScript(const std::string& name) {
        auto it = m_scripts.find(name);
        if (it != m_scripts.end()) {
            // Find the original path and reload
            // This is simplified - in practice you'd store the path
			// TODO: Implement
            m_scripts.erase(it);
        }
        return true;
    }

    void ScriptingSystem::RegisterEngineAPI() {
        // Register basic math types
        m_lua.new_usertype<XMFLOAT3>("Vector3",
            "x", &XMFLOAT3::x,
            "y", &XMFLOAT3::y,
            "z", &XMFLOAT3::z,
            sol::constructors<XMFLOAT3(), XMFLOAT3(float, float, float)>()
        );

        // Register utility functions
        m_lua["print"] = [](const std::string& msg) {
            std::cout << "[Lua] " << msg << std::endl;
        };

        m_lua["log"] = [](const std::string& msg) {
            std::cout << "[Lua Log] " << msg << std::endl;
        };
    }

    void ScriptingSystem::RegisterEntityAPI() {
        // Register Entity type
        m_lua.new_usertype<Entity>("Entity");

        // Entity management functions
        m_lua["CreateEntity"] = [this]() -> Entity {
            return m_registry->CreateEntity();
        };

        m_lua["DestroyEntity"] = [this](Entity entity) {
            m_registry->DestroyEntity(entity);
        };

        m_lua["IsValidEntity"] = [this](Entity entity) -> bool {
            return m_registry->IsValid(entity);
        };
    }

    void ScriptingSystem::RegisterComponentAPI() {
        // Register TransformComponent
        m_lua.new_usertype<TransformComponent>("Transform",
            "position", &TransformComponent::position,
            "rotation", &TransformComponent::rotation,
            "scale", &TransformComponent::scale,
            sol::constructors<TransformComponent()>()
        );

        // Component management functions
        m_lua["AddTransform"] = [this](Entity entity) -> TransformComponent& {
            return m_registry->AddComponent<TransformComponent>(entity);
        };

        m_lua["GetTransform"] = [this](Entity entity) -> TransformComponent& {
            return m_registry->GetComponent<TransformComponent>(entity);
        };

        m_lua["HasTransform"] = [this](Entity entity) -> bool {
            return m_registry->HasComponent<TransformComponent>(entity);
        };

        // Register NameComponent
        m_lua.new_usertype<NameComponent>("Name",
            "name", &NameComponent::name,
            sol::constructors<NameComponent(), NameComponent(const std::string&)>()
        );

        m_lua["AddName"] = [this](Entity entity, const std::string& name) -> NameComponent& {
            return m_registry->AddComponent<NameComponent>(entity, name);
        };

        m_lua["GetName"] = [this](Entity entity) -> NameComponent& {
            return m_registry->GetComponent<NameComponent>(entity);
        };
    }
}

