#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"

namespace SparkEngine {
    GraphicsEngine::GraphicsEngine()
        : m_swapChain(nullptr) {}

    GraphicsEngine::~GraphicsEngine() {
        Shutdown();
    }

    bool GraphicsEngine::Initialize(HWND hwnd, int width, int height) {
        Logger::Info("Initializing GraphicsEngine...");
        if (!CreateDeviceAndContext()) {
            Logger::Error("Failed to create D3D device and context");
            return false;
        }
        if (!CreateSwapChain(hwnd, width, height)) {
            Logger::Error("Failed to create swap chain");
            return false;
        }
        Logger::Info("GraphicsEngine initialized successfully");
        return true;
    }

    void GraphicsEngine::Shutdown() {
        if (m_swapChain) {
            m_swapChain->Cleanup();
            delete m_swapChain;
            m_swapChain = nullptr;
        }
        m_context.Reset();
        m_device.Reset();
        Logger::Info("GraphicsEngine shutdown complete");
    }

    void GraphicsEngine::BeginFrame(const Float4& clearColor) {
        m_context->ClearRenderTargetView(m_swapChain->GetRenderTargetView(), &clearColor.x);
        m_context->ClearDepthStencilView(m_swapChain->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void GraphicsEngine::EndFrame() {
        m_swapChain->Present();
    }

    bool GraphicsEngine::CreateDeviceAndContext() {
        UINT createFlags = 0;
    #ifdef _DEBUG
        createFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        D3D_FEATURE_LEVEL obtained;
        HRESULT hr = D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createFlags,
            featureLevels, 1, D3D11_SDK_VERSION,
            m_device.GetAddressOf(), &obtained, m_context.GetAddressOf()
        );
        return SUCCEEDED(hr);
    }

    bool GraphicsEngine::CreateSwapChain(HWND hwnd, int width, int height) {
        m_swapChain = new SwapChain();
        return m_swapChain->Initialize(m_device.Get(), m_context.Get(), hwnd, width, height);
    }

    Shader* GraphicsEngine::CreateShader(const std::wstring& filename) {
        auto shader = std::make_unique<Shader>(m_device.Get(), filename);
        Shader* ptr = shader.get();
        m_shaders.push_back(std::move(shader));
        return ptr;
    }

    Mesh* GraphicsEngine::CreateMesh(const std::string& name) {
        auto mesh = std::make_unique<Mesh>(m_device.Get(), name);
        Mesh* ptr = mesh.get();
        m_meshes.push_back(std::move(mesh));
        return ptr;
    }

    Material* GraphicsEngine::CreateMaterial(const std::string& name) {
        auto material = std::make_unique<Material>(m_device.Get(), name);
        Material* ptr = material.get();
        m_materials.push_back(std::move(material));
        return ptr;
    }
}
