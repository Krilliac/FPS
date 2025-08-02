#include "GraphicsEngine.h"
#include "Utils/Assert.h"
#include "Terrain/TerrainManager.h"
#include <iostream>
#include <algorithm>

namespace SparkEngine {
    GraphicsEngine::GraphicsEngine() {
        ZeroMemory(&m_viewport, sizeof(m_viewport));
        ZeroMemory(&m_renderStats, sizeof(m_renderStats));
    }

    GraphicsEngine::~GraphicsEngine() {
        Shutdown();
    }

    HRESULT GraphicsEngine::Initialize(HWND hWnd) {
        SPARK_ASSERT(hWnd != nullptr);
        m_hWnd = hWnd;

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        m_windowWidth = clientRect.right - clientRect.left;
        m_windowHeight = clientRect.bottom - clientRect.top;

        HRESULT hr = S_OK;

        if (FAILED(hr = CreateDevice())) {
            std::cerr << "Failed to create D3D11 device" << std::endl;
            return hr;  
        }

        if (FAILED(hr = CreateSwapChain(hWnd))) {
            std::cerr << "Failed to create swap chain" << std::endl;
            return hr;
        }

        if (FAILED(hr = CreateRenderTargetView())) {
            std::cerr << "Failed to create render target view" << std::endl;
            return hr;
        }

        if (FAILED(hr = CreateDepthStencilBuffer())) {
            std::cerr << "Failed to create depth stencil buffer" << std::endl;
            return hr;
        }

        InitializeViewport();

        if (m_settings.enableDebugLayer) {
            SetupDebugLayer();
        }

        // Initialize terrain manager
        m_terrainManager = std::make_unique<TerrainManager>(this, nullptr);
        m_terrainManager->Initialize();

        std::cout << "GraphicsEngine initialized successfully" << std::endl;
        std::cout << "Feature Level: " << std::hex << m_featureLevel << std::dec << std::endl;
        std::cout << "Resolution: " << m_windowWidth << "x" << m_windowHeight << std::endl;

        return S_OK;
    }

    void GraphicsEngine::Shutdown() {
        m_terrainManager.reset();
        ReleaseResources();

        if (m_debug) {
            m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        }

        m_debug.Reset();
        m_infoQueue.Reset();
        m_context.Reset();
        m_device.Reset();
        m_dxgiFactory.Reset();
    }

    void GraphicsEngine::BeginFrame() {
        SPARK_ASSERT(m_context && m_renderTargetView && m_depthStencilView);

        // Clear render targets
        const float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), 
                                       D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        // Set render targets
        ID3D11RenderTargetView* rtvs[] = { m_renderTargetView.Get() };
        m_context->OMSetRenderTargets(1, rtvs, m_depthStencilView.Get());

        // Set viewport
        m_context->RSSetViewports(1, &m_viewport);

        // Reset stats
        ResetRenderStats();

        BeginEvent("Frame");
    }

    void GraphicsEngine::EndFrame() {
        EndEvent();
    }

    void GraphicsEngine::Present() {
        SPARK_ASSERT(m_swapChain);
        
        UINT syncInterval = m_settings.enableVSync ? 1 : 0;
        UINT flags = 0;
        
        HRESULT hr = m_swapChain->Present(syncInterval, flags);
        
        if (FAILED(hr)) {
            if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
                std::cerr << "Device lost, attempting recovery..." << std::endl;
                // Device recovery logic would go here
            }
        }
    }

    void GraphicsEngine::OnResize(UINT width, UINT height) {
        if (width == 0 || height == 0) return;
        if (width == m_windowWidth && height == m_windowHeight) return;

        m_windowWidth = width;
        m_windowHeight = height;

        // Release dependent resources
        m_context->OMSetRenderTargets(0, nullptr, nullptr);
        m_renderTargetView.Reset();
        m_backBufferSRV.Reset();
        m_depthStencilView.Reset();
        m_depthStencilBuffer.Reset();
        m_backBuffer.Reset();

        // Resize swap chain
        HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        SPARK_ASSERT_MSG(SUCCEEDED(hr), "Failed to resize swap chain buffers");

        // Recreate resources
        CreateRenderTargetView();
        CreateDepthStencilBuffer();
        InitializeViewport();

        std::cout << "Resized to: " << width << "x" << height << std::endl;
    }

    HRESULT GraphicsEngine::CreateDevice() {
        UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        
#ifdef _DEBUG
        if (m_settings.enableDebugLayer) {
            createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        }
#endif

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> immediateContext;

        HRESULT hr = D3D11CreateDevice(
            nullptr,                    // Adapter
            D3D_DRIVER_TYPE_HARDWARE,   // Driver type
            nullptr,                    // Software
            createDeviceFlags,          // Flags
            featureLevels,              // Feature levels
            ARRAYSIZE(featureLevels),   // Num feature levels
            D3D11_SDK_VERSION,          // SDK version
            &device,                    // Device
            &m_featureLevel,            // Feature level
            &immediateContext           // Immediate context
        );

        if (FAILED(hr)) return hr;

        // Get D3D11.1 interfaces
        hr = device.As(&m_device);
        if (FAILED(hr)) return hr;

        hr = immediateContext.As(&m_context);
        if (FAILED(hr)) return hr;

        // Get DXGI factory
        ComPtr<IDXGIDevice> dxgiDevice;
        hr = m_device.As(&dxgiDevice);
        if (FAILED(hr)) return hr;

        ComPtr<IDXGIAdapter> adapter;
        hr = dxgiDevice->GetAdapter(&adapter);
        if (FAILED(hr)) return hr;

        hr = adapter->GetParent(IID_PPV_ARGS(&m_dxgiFactory));
        return hr;
    }

    HRESULT GraphicsEngine::CreateSwapChain(HWND hWnd) {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = m_windowWidth;
        swapChainDesc.Height = m_windowHeight;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = m_settings.enableMSAA ? m_settings.msaaSamples : 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        return m_dxgiFactory->CreateSwapChainForHwnd(
            m_device.Get(),
            hWnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &m_swapChain
        );
    }

    HRESULT GraphicsEngine::CreateRenderTargetView() {
        HRESULT hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer));
        if (FAILED(hr)) return hr;

        hr = m_device->CreateRenderTargetView(m_backBuffer.Get(), nullptr, &m_renderTargetView);
        if (FAILED(hr)) return hr;

        // Create shader resource view for back buffer (for post-processing)
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        hr = m_device->CreateShaderResourceView(m_backBuffer.Get(), &srvDesc, &m_backBufferSRV);
        return hr;
    }

    HRESULT GraphicsEngine::CreateDepthStencilBuffer() {
        D3D11_TEXTURE2D_DESC depthBufferDesc = {};
        depthBufferDesc.Width = m_windowWidth;
        depthBufferDesc.Height = m_windowHeight;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = m_settings.enableMSAA ? m_settings.msaaSamples : 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        HRESULT hr = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
        if (FAILED(hr)) return hr;

        return m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, &m_depthStencilView);
    }

    void GraphicsEngine::InitializeViewport() {
        m_viewport.TopLeftX = 0.0f;
        m_viewport.TopLeftY = 0.0f;
        m_viewport.Width = static_cast<float>(m_windowWidth);
        m_viewport.Height = static_cast<float>(m_windowHeight);
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
    }

    void GraphicsEngine::SetViewport(const D3D11_VIEWPORT& viewport) {
        m_viewport = viewport;
        m_context->RSSetViewports(1, &m_viewport);
    }

    void GraphicsEngine::SetDebugName(ID3D11DeviceChild* resource, const std::string& name) {
        if (resource) {
            resource->SetPrivateData(WKPDID_D3DDebugObjectName, 
                                   static_cast<UINT>(name.length()), name.c_str());
        }
    }

    void GraphicsEngine::BeginEvent(const std::string& name) {
        if (m_context) {
            std::wstring wideName(name.begin(), name.end());
            m_context->BeginEventInt(wideName.c_str(), 0);
        }
    }

    void GraphicsEngine::EndEvent() {
        if (m_context) {
            m_context->EndEvent();
        }
    }

    void GraphicsEngine::ResetRenderStats() {
        m_renderStats.drawCalls = 0;
        m_renderStats.triangles = 0;
        m_renderStats.vertices = 0;
    }

    void GraphicsEngine::ReleaseResources() {
        m_backBufferSRV.Reset();
        m_depthStencilView.Reset();
        m_depthStencilBuffer.Reset();
        m_renderTargetView.Reset();
        m_backBuffer.Reset();
        m_swapChain.Reset();
    }
}
