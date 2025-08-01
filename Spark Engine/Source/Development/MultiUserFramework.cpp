#include "MultiUserFramework.h"
#include "../Utils/Timer.h"
#include <iostream>
#include <algorithm>

namespace SparkEngine {
    MultiUserFramework::MultiUserFramework() {
        m_currentDeveloper = "Developer_" + std::to_string(GetTickCount64());
    }

    MultiUserFramework::~MultiUserFramework() {
        Shutdown();
    }

    bool MultiUserFramework::Initialize(const std::string& developerId, bool isHost) {
        m_currentDeveloper = developerId;
        m_isSessionHost = isHost;
        
        m_networkCore = std::make_unique<NetworkCore>();
        
        if (isHost) {
            if (!m_networkCore->StartHost(7777, 32)) {
                std::cerr << "Failed to start host" << std::endl;
                return false;
            }
        }
        
        // Setup event handlers
        m_networkCore->OnPacketReceived = [this](const NetworkPacket& packet) {
            HandleNetworkMessage(packet.data);
        };
        
        std::cout << "MultiUserFramework initialized for developer: " << developerId << std::endl;
        return true;
    }

    void MultiUserFramework::Shutdown() {
        if (m_networkCore) {
            if (m_isSessionHost) {
                m_networkCore->StopHost();
            } else {
                m_networkCore->DisconnectFromHost();
            }
        }
    }

    void MultiUserFramework::Update() {
        if (m_networkCore) {
            m_networkCore->Update();
        }
        ProcessIncomingEvents();
    }

    bool MultiUserFramework::CreateDevelopmentSession(const std::string& sessionId) {
        if (!m_isSessionHost) return false;
        
        std::cout << "Created development session: " << sessionId << std::endl;
        return true;
    }

    bool MultiUserFramework::JoinDevelopmentSession(const std::string& sessionId, const std::string& hostAddress) {
        if (m_isSessionHost) return false;
        
        bool connected = m_networkCore->ConnectToHost(hostAddress, 7777);
        if (connected) {
            std::cout << "Joined development session: " << sessionId << std::endl;
        }
        return connected;
    }

    bool MultiUserFramework::LockResource(const std::string& resourceId) {
        std::lock_guard<std::mutex> lock(m_lockMutex);
        
        auto it = m_resourceLocks.find(resourceId);
        if (it != m_resourceLocks.end() && it->second != m_currentDeveloper) {
            return false; // Already locked by another developer
        }
        
        m_resourceLocks[resourceId] = m_currentDeveloper;
        
        // Broadcast lock event
        DevelopmentEvent event;
        event.type = DevelopmentEvent::EntityModified;
        event.developerId = m_currentDeveloper;
        event.targetId = resourceId;
        event.timestamp = GetTickCount64();
        SendEvent(event);
        
        return true;
    }

    bool MultiUserFramework::UnlockResource(const std::string& resourceId) {
        std::lock_guard<std::mutex> lock(m_lockMutex);
        
        auto it = m_resourceLocks.find(resourceId);
        if (it != m_resourceLocks.end() && it->second == m_currentDeveloper) {
            m_resourceLocks.erase(it);
            return true;
        }
        return false;
    }

    void MultiUserFramework::ProcessIncomingEvents() {
        std::lock_guard<std::mutex> lock(m_eventMutex);
        while (!m_eventQueue.empty()) {
            const auto& event = m_eventQueue.front();
            if (OnEventReceived) {
                OnEventReceived(event);
            }
            m_eventQueue.pop();
        }
    }

    void MultiUserFramework::SendEvent(const DevelopmentEvent& event) {
        if (m_networkCore) {
            NetworkPacket packet;
            packet.type = MessageType::DeveloperAction;
            packet.senderId = m_currentDeveloper;
            packet.timestamp = event.timestamp;
            // Serialize event data here
            m_networkCore->BroadcastPacket(packet);
        }
    }

    void MultiUserFramework::HandleNetworkMessage(const std::vector<uint8_t>& message) {
        // Deserialize and handle incoming development events
        std::lock_guard<std::mutex> lock(m_eventMutex);
        
        DevelopmentEvent event;
        // Deserialize message into event
        event.timestamp = GetTickCount64();
        
        m_eventQueue.push(event);
    }
}
