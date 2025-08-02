#pragma once
#include "../Core/framework.h"
#include "../Utils/Logger.h"
#include <winsock2.h>
#pragma comment(lib, \"ws2_32.lib\")

namespace SparkEngine {
    class NetworkingSystem {
    public:
        NetworkingSystem();
        ~NetworkingSystem();

        bool Initialize();
        void Shutdown();

        bool Connect(const std::string& host, unsigned short port);
        void Send(const std::vector<uint8_t>& data);
        std::vector<uint8_t> Receive();

    private:
        WSADATA        m_wsaData;
        SOCKET         m_socket = INVALID_SOCKET;
    };
}
