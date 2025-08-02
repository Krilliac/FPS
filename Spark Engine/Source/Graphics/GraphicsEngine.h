// GraphicsEngine.h - Enhanced DirectX 11 Implementation
#pragma once

#include "Utils/Assert.h"
#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../Core/framework.h"

// ComPtr<using> declaration removed - defined in framework.h

namespace SparkEngine {
    struct RenderStats {
        uint32_t drawCalls = 0;
        uint32_t triangles = 0;
        uint32_t vertices = 0;
        float frameTime = 0.0f;
        float gpuTime = 0.0f;
    };

    struct GraphicsSettings {
        bool enableVSync = true;
        bool enableMSAA = false;
        uint32_t msaaSamples = 4;
        bool enableHDR = false;
        bool enableDebugLayer = false;
        uint32_t maxFrameLatency = 3;
    };

    class GraphicsEngine {
    public:
        GraphicsEngine();
        ~GraphicsEngine();

        // Core functionality
        HRESULT Initialize(HWND hWnd);
        void Shutdown();
        void BeginFrame();
        void EndFrame();
        void Present();

        // Window management
        void OnResize(UINT width, UINT height);
        void SetFullscreen(bool fullscreen);

        // Device accessors
        ID3D11Device1* GetDevice() const { return m_device.Get(); }
        ID3D11DeviceContext1* GetContext() const { return m_context.Get(); }
        IDXGISwapChain1* GetSwapChain() const { return m_swapChain.Get(); }

        // Render targets
        ID3D11RenderTargetView* GetBackBufferRTV() const { return m_renderTargetView.Get(); }
        ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView.Get(); }
        ID3D11ShaderResourceView* GetRenderTargetSRV() const { return m_backBufferSRV.Get(); }

        // Viewport management
        void SetViewport(const D3D11_VIEWPORT& viewport);
        void SetViewport(float x, float y, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);
        D3D11_VIEWPORT GetViewport() const { return m_viewport; }

        // Render state management
        void SetRenderTargets(UINT numViews, ID3D11RenderTargetView* const* renderTargetViews, 
                             ID3D11DepthStencilView* depthStencilView);
        void ClearRenderTarget(ID3D11RenderTargetView* rtv, const float clearColor[4]);
        void ClearDepthStencil(ID3D11DepthStencilView* dsv, float depth = 1.0f, UINT8 stencil = 0);

        // Resource creation helpers
        HRESULT CreateBuffer(const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* initialData, 
                           ID3D11Buffer** buffer);
        HRESULT CreateTexture2D(const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* initialData,
                              ID3D11Texture2D** texture);
        HRESULT CreateShaderResourceView(ID3D11Resource* resource, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc,
                                       ID3D11ShaderResourceView** srv);

        // Debug features
        void SetDebugName(ID3D11DeviceChild* resource, const std::string& name);
        void BeginEvent(const std::string& name);
        void EndEvent();

        // Statistics
        const RenderStats& GetRenderStats() const { return m_renderStats; }
        void ResetRenderStats();

        // Settings
        void SetGraphicsSettings(const GraphicsSettings& settings) { m_settings = settings; }
        const GraphicsSettings& GetGraphicsSettings() const { return m_settings; }

        // Advanced features
        bool SupportsFeatureLevel(D3D_FEATURE_LEVEL level) const;
        bool SupportsFormat(DXGI_FORMAT format, UINT usage) const;
        void EnableCollaborativeVisualization(bool enable) { m_collaborativeVisualization = enable; }
        void RenderDeveloperCursors(const std::vector<XMFLOAT3>& positions, 
                                   const std::vector<XMFLOAT4>& colors);

        // Window properties
        UINT GetWindowWidth() const { return m_windowWidth; }
        UINT GetWindowHeight() const { return m_windowHeight; }
        float GetAspectRatio() const { return static_cast<float>(m_windowWidth) / m_windowHeight; }

    private:
        // Initialization helpers
        HRESULT CreateDevice();
        HRESULT CreateSwapChain(HWND hWnd);
        HRESULT CreateRenderTargetView();
        HRESULT CreateDepthStencilBuffer();
        HRESULT SetupDebugLayer();
        void InitializeViewport();

        // Resource management
        void ReleaseResources();
        void ResizeBuffers();

        // Device and swap chain
        ComPtr<ID3D11Device1> m_device;
        ComPtr<ID3D11DeviceContext1> m_context;
        ComPtr<IDXGISwapChain1> m_swapChain;
        ComPtr<IDXGIFactory2> m_dxgiFactory;

        // Render targets and depth buffer
        ComPtr<ID3D11Texture2D> m_backBuffer;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11ShaderResourceView> m_backBufferSRV;
        ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        // Debug interface
        ComPtr<ID3D11Debug> m_debug;
        ComPtr<IDXGIInfoQueue> m_infoQueue;

        // Window properties
        HWND m_hWnd = nullptr;
        UINT m_windowWidth = 1280;
        UINT m_windowHeight = 720;
        bool m_isFullscreen = false;

        // Viewport
        D3D11_VIEWPORT m_viewport = {};

        // Settings and stats
        GraphicsSettings m_settings;
        RenderStats m_renderStats;
        D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

        // Collaborative features
        bool m_collaborativeVisualization = false;
        std::unique_ptr<class TerrainManager> m_terrainManager;
    };
}


