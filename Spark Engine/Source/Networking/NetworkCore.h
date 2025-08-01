#pragma once
#include "../Core/framework.h"
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

namespace SparkEngine {
    enum class MessageType : uint8_t {
        SessionJoin, SessionLeave, EntitySync, ContentUpdate,
        ScriptExecution, AssetRequest, DeveloperAction, SystemEvent
    };

    struct NetworkPacket {
        MessageType type;
        std::string senderId;
        std::string targetId; // Empty for broadcast
        std::vector<uint8_t> data;
        uint64_t timestamp;
        uint32_t sequenceNumber;
        bool requiresAck = false;
    };

    class NetworkCore {
    private:
        // Connection management
        std::unique_ptr<std::thread> m_networkThread;
        std::atomic<bool> m_isRunning{false};
        std::atomic<bool> m_isHost{false};
        
        // Client management
        std::unordered_map<std::string, void*> m_connections; // Platform-specific handles
        std::mutex m_connectionMutex;
        
        // Message queues
        std::queue<NetworkPacket> m_incomingPackets;
        std::queue<NetworkPacket> m_outgoingPackets;
        std::mutex m_packetMutex;
        
        // Network statistics
        struct NetworkMetrics {
            uint64_t bytesSent = 0;
            uint64_t bytesReceived = 0;
            uint32_t packetsSent = 0;
            uint32_t packetsReceived = 0;
            float currentLatency = 0.0f;
            float packetLoss = 0.0f;
        } m_metrics;

        uint32_t m_nextSequenceNumber = 1;

    public:
        NetworkCore();
        ~NetworkCore();

        // Host functionality
        bool StartHost(uint16_t port, uint32_t maxConnections = 32);
        void StopHost();
        
        // Client functionality
        bool ConnectToHost(const std::string& hostAddress, uint16_t port);
        void DisconnectFromHost();
        
        // Message handling
        void Update();
        void SendPacket(const NetworkPacket& packet);
        void SendPacketToClient(const std::string& clientId, const NetworkPacket& packet);
        void BroadcastPacket(const NetworkPacket& packet, const std::string& excludeClient = "");
        
        bool HasIncomingPackets() const;
        NetworkPacket GetNextPacket();
        
        // Connection management
        void RegisterConnection(const std::string& connectionId, const std::string& connectionInfo);
        void UnregisterConnection(const std::string& connectionId);
        std::vector<std::string> GetActiveConnections() const;
        
        // Network statistics
        const NetworkMetrics& GetNetworkMetrics() const { return m_metrics; }
        
        // Event callbacks
        std::function<void(const std::string&)> OnConnectionEstablished;
        std::function<void(const std::string&)> OnConnectionLost;
        std::function<void(const NetworkPacket&)> OnPacketReceived;

    private:
        void NetworkThreadFunction();
        void HandleNewConnection(void* socket);
        void HandleClientPacket(const std::string& clientId, const NetworkPacket& packet);
        void ProcessIncomingData();
        void ProcessOutgoingData();
        
        // Serialization
        std::vector<uint8_t> SerializePacket(const NetworkPacket& packet);
        NetworkPacket DeserializePacket(const std::vector<uint8_t>& data);
    };
}
