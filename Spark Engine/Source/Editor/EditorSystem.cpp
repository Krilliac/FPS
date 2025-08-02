#include "EditorSystem.h"
#include "../Utils/Logger.h"
#ifdef IMGUI_AVAILABLE
#include "ImGuiImpl.h"
#endif

namespace SparkEngine {
#ifdef IMGUI_AVAILABLE
    EditorSystem::EditorSystem(GraphicsEngine& graphics)
        : m_graphics(graphics) {}

    EditorSystem::~EditorSystem() { Shutdown(); }

    bool EditorSystem::Initialize() {
        Logger::Info("Initializing EditorSystem...");
        ImGuiImpl::Init(m_graphics);
        return true;
    }

    void EditorSystem::Shutdown() {
        Logger::Info("Shutting down EditorSystem...");
        ImGuiImpl::Shutdown();
    }

    void EditorSystem::Update() {
        ImGuiImpl::NewFrame();
        ImGui::Begin("Editor Controls");
        if (ImGui::Button("Toggle Demo")) {
            m_showDemo = !m_showDemo;
        }
        ImGui::End();
    }

    void EditorSystem::Render() {
        if (m_showDemo) {
            ImGui::ShowDemoWindow(&m_showDemo);
        }
        ImGuiImpl::Render();
    }
#endif
}
