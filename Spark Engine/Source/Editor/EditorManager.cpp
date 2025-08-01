// EditorManager.cpp
#include "EditorManager
#include "Windows/CollaborationWindow.h".h"
#include "../Graphics/RenderThreadManager.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <algorithm>

namespace SparkEngine {

bool EditorManager::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context, EntityRegistry* registry) {
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);

    m_registry = registry;
    m_device   = device;
    m_context  = context;
    m_initialized = true;

    // Register default editor windows
    AddWindow<DockSpaceWindow>();
    AddWindow<ToolbarWindow>();
    AddWindow<SceneHierarchyWindow>(m_registry, [this](Entity e) { SelectEntity(e); });
    AddWindow<InspectorWindow>(m_registry);
    AddWindow<AssetBrowserWindow>();
    if (m_collaborativeDevelopment) {
        AddWindow<CollaborationWindow>(m_collaborativeDevelopment);
    }
    AddWindow<ViewportWindow>(m_graphics);

    return true;
}

void EditorManager::Shutdown() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    m_windows.clear();
}

void EditorManager::NewFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void EditorManager::Render() {
    // Render docking space first
    for (auto& win : m_windows) {
        if (strcmp(win->GetName(), "DockSpace") == 0) {
            win->Render();
            break;
        }
    }

    // Render other windows
    for (auto& win : m_windows) {
        const char* name = win->GetName();
        if (strcmp(name, "DockSpace") != 0) {
            win->Render();
        }
    }

    // Show ImGui demo if requested
    if (m_showDemoWindow) {
        ImGui::ShowDemoWindow(&m_showDemoWindow);
    }

    ImGui::Render();
}

void EditorManager::Present() {
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EditorManager::SelectEntity(Entity entity) {
    m_selectedEntity = entity;
    for (auto& win : m_windows) {
        win->OnEntitySelected(entity);
    }
}

void EditorManager::RemoveWindow(const char* name) {
    m_windows.erase(
        std::remove_if(m_windows.begin(), m_windows.end(),
            [&](auto& w) { return strcmp(w->GetName(), name) == 0; }),
        m_windows.end()
    );
}

EditorWindow* EditorManager::GetWindow(const char* name) {
    for (auto& w : m_windows) {
        if (strcmp(w->GetName(), name) == 0) {
            return w.get();
        }
    }
    return nullptr;
}

bool EditorManager::IsMouseOverEditor() const {
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool EditorManager::WantCaptureKeyboard() const {
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

} // namespace SparkEngine

