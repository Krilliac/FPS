#pragma once
#include "../Core/framework.h"
#include "../Networking/NetworkCore.h"
#include <unordered_map>
#include <functional>
#include <mutex>

namespace SparkEngine {
    struct DevelopmentEvent {
        enum Type { EntityCreated, EntityModified, EntityDeleted, SceneChanged, AssetModified, PropertyChanged };
        Type type;
        uint64_t timestamp;
        std::string developerId;
        std::string targetId;
        std::vector<uint8_t> payload;
        Entity entityId = entt::null;
    };

    struct DeveloperSession {
        std::string developerId;
        std::string displayName;
        DirectX::XMFLOAT4 cursorColor;
        DirectX::XMFLOAT3 viewPosition;
        bool isActive = true;
        uint64_t lastActivity = 0;
    };

    class MultiUserFramework {
    private:
        std::unique_ptr<class NetworkCore> m_networkCore;
        std::queue<DevelopmentEvent> m_eventQueue;
        std::mutex m_eventMutex;
        std::unordered_map<std::string, DeveloperSession> m_activeDevelopers;
        std::string m_currentDeveloper;
        bool m_isSessionHost = false;
        
        // Resource locking system
        std::unordered_map<std::string, std::string> m_resourceLocks;
        std::mutex m_lockMutex;

    public:
        MultiUserFramework();
        ~MultiUserFramework();

        bool Initialize(const std::string& developerId, bool isHost = false);
        void Shutdown();
        void Update();

        // Session management
        bool CreateDevelopmentSession(const std::string& sessionId);
        bool JoinDevelopmentSession(const std::string& sessionId, const std::string& hostAddress);
        void LeaveDevelopmentSession();

        // Resource locking
        bool LockResource(const std::string& resourceId);
        bool UnlockResource(const std::string& resourceId);
        bool IsResourceLocked(const std::string& resourceId, std::string* lockedBy = nullptr);
        
        // Event broadcasting
        void BroadcastEntityModification(Entity entity, const std::string& component);
        void BroadcastSceneModification(const std::string& sceneName);
        void BroadcastAssetModification(const std::string& assetId);

        // Developer management
        std::vector<DeveloperSession> GetActiveDevelopers() const;
        bool IsDeveloperActive(const std::string& developerId) const;
        void UpdateDeveloperPosition(const DirectX::XMFLOAT3& position);

        // Event handling
        std::function<void(const DevelopmentEvent&)> OnEventReceived;
        std::function<void(const DeveloperSession&)> OnDeveloperJoined;
        std::function<void(const std::string&)> OnDeveloperLeft;

    private:
        void ProcessIncomingEvents();
        void SendEvent(const DevelopmentEvent& event);
        void HandleNetworkMessage(const std::vector<uint8_t>& message);
    };
}
