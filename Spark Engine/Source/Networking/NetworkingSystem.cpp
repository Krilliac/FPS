#include \"NetworkingSystem.h\"

namespace SparkEngine {
    NetworkingSystem::NetworkingSystem() {}
    NetworkingSystem::~NetworkingSystem() { Shutdown(); }

    bool NetworkingSystem::Initialize() {
        Logger::Info(\"Initializing NetworkingSystem...\");
        if (WSAStartup(MAKEWORD(2,2), &m_wsaData) != 0) {
            Logger::Error(\"WSAStartup failed\");
            return false;
        }
        return true;
    }

    void NetworkingSystem::Shutdown() {
        Logger::Info(\"Shutting down NetworkingSystem...\");
        if (m_socket != INVALID_SOCKET) {
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
        }
        WSACleanup();
    }

    bool NetworkingSystem::Connect(const std::string& host, unsigned short port) {
        addrinfo hints = {}, *result = nullptr;
        hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM; hints.ai_protocol = IPPROTO_TCP;
        if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &result) != 0) {
            Logger::Error(\"getaddrinfo failed\");
            return false;
        }
        m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (m_socket == INVALID_SOCKET) {
            Logger::Error(\"Socket creation failed\");
            freeaddrinfo(result);
            return false;
        }
        if (connect(m_socket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
            Logger::Error(\"Connection failed\");
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            freeaddrinfo(result);
            return false;
        }
        freeaddrinfo(result);
        Logger::Info(\"Connected to \" + host + \":\" + std::to_string(port));
        return true;
    }

    void NetworkingSystem::Send(const std::vector<uint8_t>& data) {
        if (m_socket == INVALID_SOCKET) return;
        send(m_socket, reinterpret_cast<const char*>(data.data()), (int)data.size(), 0);
    }

    std::vector<uint8_t> NetworkingSystem::Receive() {
        std::vector<uint8_t> buffer(1024);
        int bytes = recv(m_socket, reinterpret_cast<char*>(buffer.data()), (int)buffer.size(), 0);
        if (bytes <= 0) return {};
        buffer.resize(bytes);
        return buffer;
    }
}
