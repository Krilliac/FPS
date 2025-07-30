// GraphicsEngine.h
#pragma once

#include "Utils/Assert.h"
#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <dxgi1_3.h>     // IDXGIFactory2, IDXGISwapChain1
#include <dxgidebug.h>   // DXGIGetDebugInterface1, IDXGIInfoQueue
#include "..\Core\framework.h"  // XMFLOAT3, XMMATRIX, HRESULT

using Microsoft::WRL::ComPtr;

class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    // Initialize device, swap chain, RTV/DSV, debug filters
    HRESULT Initialize(HWND hWnd);

    // Cleanup
    void Shutdown();

    // Frame operations
    void BeginFrame();
    void EndFrame();

    // Handle window resize
    void OnResize(UINT width, UINT height);

    // Accessors
    ID3D11Device* GetDevice()     const { return m_device.Get(); }
    ID3D11DeviceContext* GetContext()    const { return m_context.Get(); }
    UINT                 GetWindowWidth() const { return m_windowWidth; }
    UINT                 GetWindowHeight() const { return m_windowHeight; }

private:
    HRESULT CreateDeviceAndSwapChain(HWND hWnd);
    HRESULT CreateRenderTargetView();
    HRESULT CreateDepthStencilView();
    void    SetViewport();

    ComPtr<ID3D11Device1>          m_device;
    ComPtr<ID3D11DeviceContext1>   m_context;
    ComPtr<IDXGISwapChain1>        m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;

    UINT m_windowWidth;
    UINT m_windowHeight;
};