#include "EditorManager.h"
#include "../Graphics/RenderThreadManager.h"
#include <imgui.h>

namespace SparkEngine {
    bool EditorManager::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context, EntityRegistry* registry) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(device, context);
        m_registry = registry;
        m_initialized = true;
        return true;
    }

    void EditorManager::Shutdown() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorManager::NewFrame() {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void EditorManager::Render() {
        RenderDockSpace();
        RenderMainMenuBar();
        for (auto& w : m_windows) { w->Render(); }
        if (m_showDemoWindow) ImGui::ShowDemoWindow(&m_showDemoWindow);
        ImGui::Render();
    }

    void EditorManager::Present() {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void EditorManager::SelectEntity(Entity e) {
        m_selectedEntity = e;
        for (auto& w : m_windows) { w->OnEntitySelected(e); }
    }

    void EditorManager::RenderMainMenuBar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Editor")) {
                ImGui::MenuItem("Demo Window", nullptr, &m_showDemoWindow);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void EditorManager::RenderDockSpace() {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    }

    void EditorManager::RemoveWindow(const char* name) {
        m_windows.erase(
            std::remove_if(m_windows.begin(), m_windows.end(),
                [&](auto& w){ return strcmp(w->GetName(), name)==0; }
            ),
            m_windows.end()
        );
    }

    EditorWindow* EditorManager::GetWindow(const char* name) {
        for (auto& w : m_windows) if (strcmp(w->GetName(), name)==0) return w.get();
        return nullptr;
    }
}