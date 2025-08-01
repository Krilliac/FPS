#include "NetworkCore.h"
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace SparkEngine {
    NetworkCore::NetworkCore() {}

    NetworkCore::~NetworkCore() {
        if (m_isHost) {
            StopHost();
        } else {
            DisconnectFromHost();
        }
    }

    bool NetworkCore::StartHost(uint16_t port, uint32_t maxConnections) {
        m_isHost = true;
        m_isRunning = true;
        
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed" << std::endl;
            return false;
        }
#endif
        
        // Create and bind socket
        // Platform-specific socket implementation would go here
        
        m_networkThread = std::make_unique<std::thread>(&NetworkCore::NetworkThreadFunction, this);
        
        std::cout << "Network host started on port " << port << std::endl;
        return true;
    }

    void NetworkCore::StopHost() {
        m_isRunning = false;
        if (m_networkThread && m_networkThread->joinable()) {
            m_networkThread->join();
        }
        
#ifdef _WIN32
        WSACleanup();
#endif
        
        std::cout << "Network host stopped" << std::endl;
    }

    bool NetworkCore::ConnectToHost(const std::string& hostAddress, uint16_t port) {
        m_isHost = false;
        m_isRunning = true;
        
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
        
        // Connect to host implementation
        
        m_networkThread = std::make_unique<std::thread>(&NetworkCore::NetworkThreadFunction, this);
        
        std::cout << "Connected to host: " << hostAddress << ":" << port << std::endl;
        return true;
    }

    void NetworkCore::DisconnectFromHost() {
        m_isRunning = false;
        if (m_networkThread && m_networkThread->joinable()) {
            m_networkThread->join();
        }
        
        std::cout << "Disconnected from host" << std::endl;
    }

    void NetworkCore::Update() {
        ProcessIncomingData();
        ProcessOutgoingData();
    }

    void NetworkCore::SendPacket(const NetworkPacket& packet) {
        std::lock_guard<std::mutex> lock(m_packetMutex);
        m_outgoingPackets.push(packet);
    }

    void NetworkCore::BroadcastPacket(const NetworkPacket& packet, const std::string& excludeClient) {
        std::lock_guard<std::mutex> lock(m_connectionMutex);
        for (const auto& connection : m_connections) {
            if (connection.first != excludeClient) {
                SendPacketToClient(connection.first, packet);
            }
        }
    }

    bool NetworkCore::HasIncomingPackets() const {
        std::lock_guard<std::mutex> lock(m_packetMutex);
        return !m_incomingPackets.empty();
    }

    NetworkPacket NetworkCore::GetNextPacket() {
        std::lock_guard<std::mutex> lock(m_packetMutex);
        if (!m_incomingPackets.empty()) {
            NetworkPacket packet = m_incomingPackets.front();
            m_incomingPackets.pop();
            return packet;
        }
        return {};
    }

    void NetworkCore::NetworkThreadFunction() {
        while (m_isRunning) {
            // Handle network I/O
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }

    void NetworkCore::ProcessIncomingData() {
        // Process incoming network data
    }

    void NetworkCore::ProcessOutgoingData() {
        std::lock_guard<std::mutex> lock(m_packetMutex);
        while (!m_outgoingPackets.empty()) {
            const auto& packet = m_outgoingPackets.front();
            // Send packet over network
            m_outgoingPackets.pop();
        }
    }
}
