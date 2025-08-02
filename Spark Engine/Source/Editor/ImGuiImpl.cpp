#include "ImGuiImpl.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace SparkEngine {
    static ID3D11Device*        g_pd3dDevice = nullptr;
    static ID3D11DeviceContext* g_pd3dContext = nullptr;
    static IDXGISwapChain*      g_pSwapChain = nullptr;
    static HWND                 g_hWnd = nullptr;

    void ImGuiImpl::Init(GraphicsEngine& graphics) {
        g_pd3dDevice = graphics.GetSwapChain()->GetRenderTargetView()->GetDevice();
        g_pd3dContext = graphics.GetSwapChain()->GetRenderTargetView()->GetDeviceContext();
        g_pSwapChain = graphics.GetSwapChain()->GetSwapChain();
        g_hWnd = g_pSwapChain->GetDesc().OutputWindow;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dContext);
    }

    void ImGuiImpl::Shutdown() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiImpl::NewFrame() {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiImpl::Render() {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}
