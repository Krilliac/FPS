#pragma once
#include "../Core/framework.h"
#include "../ECS/EntityRegistry.h"
#include <sol/sol.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace SparkEngine {
    class ScriptingSystem {
    private:
        sol::state m_lua;
        std::unordered_map<std::string, sol::protected_function> m_scripts;
        EntityRegistry* m_registry = nullptr;

    public:
        ScriptingSystem() = default;
        ~ScriptingSystem() = default;

        bool Initialize(EntityRegistry* registry);
        void Shutdown();

        bool LoadScript(const std::string& filepath, const std::string& name);
        bool ExecuteScript(const std::string& name);
        bool ReloadScript(const std::string& name);

        template<typename... Args>
        bool CallFunction(const std::string& scriptName, const std::string& functionName, Args&&... args) {
            auto scriptIt = m_scripts.find(scriptName);
            if (scriptIt == m_scripts.end()) {
                return false;
            }

            try {
                sol::protected_function func = scriptIt->second[functionName];
                if (func.valid()) {
                    sol::protected_function_result result = func(std::forward<Args>(args)...);
                    if (!result.valid()) {
                        sol::error err = result;
                        // Log error
                        return false;
                    }
                    return true;
                }
            } catch (const std::exception& e) {
                // Log exception
                return false;
            }

            return false;
        }

        sol::state& GetLuaState() { return m_lua; }
        const sol::state& GetLuaState() const { return m_lua; }

    private:
        void RegisterEngineAPI();
        void RegisterEntityAPI();
        void RegisterComponentAPI();
    };

    // Script Component for entities
    struct ScriptComponent {
        std::string scriptPath;
        std::string scriptName;
        bool isActive = true;
        bool isInitialized = false;
        std::unordered_map<std::string, sol::object> scriptData;

        ScriptComponent() = default;
        ScriptComponent(const std::string& path, const std::string& name) 
            : scriptPath(path), scriptName(name) {}
    };
}
