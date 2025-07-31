#pragma once
#include "../Core/framework.h"
#include "../ECS/EntityRegistry.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <memory>
#include <vector>
#include <functional>

namespace SparkEngine {
    class EditorWindow {
    public:
        virtual ~EditorWindow() = default;
        virtual void Render() = 0;
        virtual const char* GetName() const = 0;
        virtual bool IsVisible() const { return m_visible; }
        virtual void SetVisible(bool visible) { m_visible = visible; }
        virtual void OnEntitySelected(Entity entity) {}

    protected:
        bool m_visible = true;
    };

    class EditorManager {
    private:
        std::vector<std::unique_ptr<EditorWindow>> m_windows;
        EntityRegistry* m_registry = nullptr;
        Entity m_selectedEntity = entt::null;
        bool m_initialized = false;
        bool m_showDemoWindow = false;

        // Editor state
        ImGuiIO* m_io = nullptr;
        float m_dockSpaceSize = 0.0f;

    public:
        EditorManager() = default;
        ~EditorManager() = default;

        bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context, EntityRegistry* registry);
        void Shutdown();

        void NewFrame();
        void Render();
        void Present();

        // Window management
        template<typename T, typename... Args>
        T* AddWindow(Args&&... args) {
            static_assert(std::is_base_of_v<EditorWindow, T>, "T must inherit from EditorWindow");

            auto window = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = window.get();
            m_windows.push_back(std::move(window));
            return ptr;
        }

        void RemoveWindow(const char* name);
        EditorWindow* GetWindow(const char* name);

        // Entity selection
        void SelectEntity(Entity entity);
        Entity GetSelectedEntity() const { return m_selectedEntity; }

        // Utility functions
        void ShowDemoWindow() { m_showDemoWindow = true; }
        bool IsMouseOverEditor() const;
        bool WantCaptureKeyboard() const;
        bool WantCaptureMouse() const;

    private:
        void RenderMainMenuBar();
        void RenderDockSpace();
        void HandleWindowEvents();
    };
}
