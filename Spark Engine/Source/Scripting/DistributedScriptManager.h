#pragma once
#include "../Core/framework.h"
#include "../Scripting/ScriptingSystem.h"
#include "../Networking/NetworkCore.h"
#include <unordered_map>

namespace SparkEngine {
    enum class ScriptExecutionContext {
        Host,        // Runs only on session host
        Client,      // Runs only on clients
        Distributed, // Runs on all participants
        Authoritative // Host has authority, clients predict
    };

    struct DistributedScript {
        std::string scriptId;
        std::string sourceCode;
        ScriptExecutionContext context;
        bool isActive = true;
        uint64_t lastModified = 0;
    };

    class DistributedScriptManager {
    private:
        ScriptingSystem* m_scriptingSystem;
        NetworkCore* m_networkCore;
        
        std::unordered_map<std::string, DistributedScript> m_scripts;
        std::unordered_map<Entity, std::vector<std::string>> m_entityScripts;
        
        // Script synchronization
        std::mutex m_scriptMutex;
        std::thread m_syncThread;
        std::atomic<bool> m_isRunning{false};

    public:
        DistributedScriptManager(ScriptingSystem* scriptingSystem, NetworkCore* networkCore);
        ~DistributedScriptManager();

        bool Initialize();
        void Shutdown();
        void Update(float deltaTime);

        // Script management
        bool LoadDistributedScript(const std::string& scriptId, const std::string& source, 
                                 ScriptExecutionContext context);
        bool UnloadDistributedScript(const std::string& scriptId);
        bool ReloadDistributedScript(const std::string& scriptId);
        
        // Script execution
        void ExecuteDistributedScript(const std::string& scriptId, Entity entity = entt::null);
        void ExecuteScriptFunction(const std::string& scriptId, const std::string& functionName, 
                                 const std::vector<std::string>& args);
        
        // Entity script binding
        void BindScriptToEntity(Entity entity, const std::string& scriptId);
        void UnbindScriptFromEntity(Entity entity, const std::string& scriptId);
        std::vector<std::string> GetEntityScripts(Entity entity) const;
        
        // Network script events
        void SendScriptEvent(const std::string& targetId, const std::string& eventName, 
                           const std::string& data);
        void BroadcastScriptEvent(const std::string& eventName, const std::string& data);
        void HandleScriptEvent(const std::string& senderId, const std::string& eventName, 
                             const std::string& data);
        
        // Script state synchronization
        void SynchronizeScriptState(const std::string& scriptId);
        void RequestScript(const std::string& scriptId);
        
        // Hot script reloading
        void NotifyScriptModified(const std::string& scriptId);
        void ReloadAllModifiedScripts();
        
        // Authority management
        bool HasScriptAuthority(const std::string& scriptId) const;
        void TransferScriptAuthority(const std::string& scriptId, const std::string& newAuthority);

        // Context filtering
        std::vector<std::string> GetScriptsByContext(ScriptExecutionContext context) const;
        bool IsValidForContext(const std::string& scriptId, ScriptExecutionContext context) const;

    private:
        void SyncThreadFunction();
        void ProcessScriptSynchronization();
        void HandleScriptUpdate(const NetworkPacket& packet);
        void CompileAndValidateScript(DistributedScript& script);
    };
}
