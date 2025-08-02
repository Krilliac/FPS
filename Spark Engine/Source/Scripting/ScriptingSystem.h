#pragma once
#include "../Core/framework.h"

namespace SparkEngine {
    class ScriptingSystem {
    public:
        ScriptingSystem()=default; ~ScriptingSystem()=default;
        bool Initialize() { return true; }
        void Shutdown() {}
        void Update(float deltaTime) {}
    private:
        bool m_initialized = false;
        std::unordered_map<std::string, std::string> m_scripts;
    };
}
