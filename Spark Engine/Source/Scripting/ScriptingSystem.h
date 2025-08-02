#pragma once
#include "../Core/framework.h"
#ifdef SOL2_AVAILABLE
#include <sol/sol.hpp>
#endif

namespace SparkEngine {
#ifdef SOL2_AVAILABLE
    class ScriptingSystem {
    public:
        ScriptingSystem();
        ~ScriptingSystem();

        bool Initialize();
        void Shutdown();
        void Update(float deltaTime);

        bool ExecuteFile(const std::string& filepath);
        bool ExecuteString(const std::string& code);

    private:
        std::unique_ptr<sol::state> m_lua;
    };
#endif
}
