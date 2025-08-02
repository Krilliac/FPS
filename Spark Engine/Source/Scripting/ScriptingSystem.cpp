#include "ScriptingSystem.h"
#include "../Utils/Logger.h"

#ifdef SOL2_AVAILABLE
namespace SparkEngine {
    ScriptingSystem::ScriptingSystem() {}
    ScriptingSystem::~ScriptingSystem() { Shutdown(); }

    bool ScriptingSystem::Initialize() {
        Logger::Info("Initializing Lua scripting...");
        m_lua = std::make_unique<sol::state>();
        m_lua->open_libraries(sol::lib::base, sol::lib::package,
                              sol::lib::math, sol::lib::table,
                              sol::lib::string, sol::lib::os);
        Logger::Info("Lua initialized");
        return true;
    }

    void ScriptingSystem::Shutdown() {
        Logger::Info("Shutting down Lua scripting...");
        m_lua.reset();
        Logger::Info("Lua shutdown complete");
    }

    void ScriptingSystem::Update(float deltaTime) {
        // Could call update hooks in Lua scripts
    }

    bool ScriptingSystem::ExecuteFile(const std::string& filepath) {
        Logger::Info("Executing Lua file: " + filepath);
        sol::load_result script = m_lua->load_file(filepath);
        if (!script.valid()) {
            sol::error err = script;
            Logger::Error(err.what());
            return false;
        }
        sol::protected_function_result result = script();
        if (!result.valid()) {
            sol::error err = result;
            Logger::Error(err.what());
            return false;
        }
        return true;
    }

    bool ScriptingSystem::ExecuteString(const std::string& code) {
        sol::load_result script = m_lua->load(code);
        if (!script.valid()) {
            sol::error err = script;
            Logger::Error(err.what());
            return false;
        }
        return ExecuteFile(code);
    }
}
#endif
