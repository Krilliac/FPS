#pragma once
#include "..\Core\framework.h"

class GraphicsEngine
{
private:
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    IDXGISwapChain* m_swapChain;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11DepthStencilView* m_depthStencilView;
    
    UINT m_windowWidth;
    UINT m_windowHeight;

public:
    GraphicsEngine();
    ~GraphicsEngine();

    HRESULT Initialize(HWND hwnd);
    void Shutdown();
    void Render();
    void OnResize(UINT width, UINT height);

    // Accessors
    ID3D11Device* GetDevice() const { return m_device; }
    ID3D11DeviceContext* GetContext() const { return m_context; }
    UINT GetWindowWidth() const { return m_windowWidth; }
    UINT GetWindowHeight() const { return m_windowHeight; }

private:
    HRESULT CreateDevice(HWND hwnd);
    HRESULT CreateRenderTargetView();
    HRESULT CreateDepthStencilView();
    void SetViewport();
};
