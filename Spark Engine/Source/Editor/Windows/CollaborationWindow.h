#pragma once
#include "../EditorManager.h"
#include "../../Development/CollaborativeDevelopment.h"

namespace SparkEngine {
    class CollaborationWindow : public EditorWindow {
    private:
        CollaborativeDevelopment* m_collaboration;
        bool m_showDeveloperList = true;
        bool m_showNetworkStats = true;
        char m_sessionIdBuffer[256] = "";
        char m_hostAddressBuffer[256] = "127.0.0.1";

    public:
        CollaborationWindow(CollaborativeDevelopment* collaboration) 
            : m_collaboration(collaboration) {
            m_visible = true;
        }

        void Render() override {
            if (!ImGui::Begin("Collaboration", &m_visible)) {
                ImGui::End();
                return;
            }

            // Session Management
            if (ImGui::CollapsingHeader("Development Session")) {
                ImGui::InputText("Session ID", m_sessionIdBuffer, sizeof(m_sessionIdBuffer));
                ImGui::InputText("Host Address", m_hostAddressBuffer, sizeof(m_hostAddressBuffer));
                
                if (ImGui::Button("Create Session")) {
                    m_collaboration->CreateDevelopmentSession(m_sessionIdBuffer);
                }
                ImGui::SameLine();
                if (ImGui::Button("Join Session")) {
                    m_collaboration->JoinDevelopmentSession(m_sessionIdBuffer, m_hostAddressBuffer);
                }
                ImGui::SameLine();
                if (ImGui::Button("Leave Session")) {
                    m_collaboration->EndDevelopmentSession();
                }
            }

            // Connected Developers
            if (ImGui::CollapsingHeader("Connected Developers")) {
                auto developers = m_collaboration->GetConnectedDevelopers();
                for (const auto& dev : developers) {
                    ImGui::Text("â€¢ %s", dev.c_str());
                }
                if (developers.empty()) {
                    ImGui::Text("No other developers connected");
                }
            }

            // Network Statistics
            if (ImGui::CollapsingHeader("Network Statistics")) {
                if (m_collaboration->GetNetworkCore()) {
                    const auto& metrics = m_collaboration->GetNetworkCore()->GetNetworkMetrics();
                    ImGui::Text("Bytes Sent: %llu", metrics.bytesSent);
                    ImGui::Text("Bytes Received: %llu", metrics.bytesReceived);
                    ImGui::Text("Packets Sent: %u", metrics.packetsSent);
                    ImGui::Text("Packets Received: %u", metrics.packetsReceived);
                    ImGui::Text("Latency: %.2f ms", metrics.currentLatency);
                }
            }

            // Collaboration Settings
            if (ImGui::CollapsingHeader("Settings")) {
                bool collabEnabled = m_collaboration->IsCollaborative();
                if (ImGui::Checkbox("Enable Collaboration", &collabEnabled)) {
                    m_collaboration->EnableRealTimeContentSync(collabEnabled);
                }
                
                bool streamingEnabled = true; // Get from streaming manager
                if (ImGui::Checkbox("Enable Asset Streaming", &streamingEnabled)) {
                    m_collaboration->EnableAssetStreaming(streamingEnabled);
                }
            }

            ImGui::End();
        }

        const char* GetName() const override { return "Collaboration"; }
    };
}
