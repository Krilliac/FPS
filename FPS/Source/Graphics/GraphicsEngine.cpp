#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
    : m_device(nullptr)
    , m_context(nullptr)
    , m_swapChain(nullptr)
    , m_renderTargetView(nullptr)
    , m_depthStencilView(nullptr)
    , m_windowWidth(1280)
    , m_windowHeight(720)
{
}

GraphicsEngine::~GraphicsEngine()
{
    Shutdown();
}

HRESULT GraphicsEngine::Initialize(HWND hwnd)
{
    HRESULT hr = S_OK;

    hr = CreateDevice(hwnd);
    if (FAILED(hr)) return hr;

    hr = CreateRenderTargetView();
    if (FAILED(hr)) return hr;

    hr = CreateDepthStencilView();
    if (FAILED(hr)) return hr;

    SetViewport();

    return S_OK;
}

void GraphicsEngine::Shutdown()
{
    if (m_depthStencilView) { m_depthStencilView->Release(); m_depthStencilView = nullptr; }
    if (m_renderTargetView) { m_renderTargetView->Release(); m_renderTargetView = nullptr; }
    if (m_swapChain) { m_swapChain->Release(); m_swapChain = nullptr; }
    if (m_context) { m_context->Release(); m_context = nullptr; }
    if (m_device) { m_device->Release(); m_device = nullptr; }
}

void GraphicsEngine::Render()
{
    // Clear render target
    float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Set render targets
    m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // TODO: Render game objects here

    // Present the frame
    m_swapChain->Present(1, 0);
}

void GraphicsEngine::OnResize(UINT width, UINT height)
{
    if (width == 0 || height == 0) return;

    m_windowWidth = width;
    m_windowHeight = height;

    // Release render target views
    if (m_renderTargetView) { m_renderTargetView->Release(); m_renderTargetView = nullptr; }
    if (m_depthStencilView) { m_depthStencilView->Release(); m_depthStencilView = nullptr; }

    // Resize swap chain
    m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    // Recreate render target views
    CreateRenderTargetView();
    CreateDepthStencilView();
    SetViewport();
}

HRESULT GraphicsEngine::CreateDevice(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    m_windowWidth = rc.right - rc.left;
    m_windowHeight = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = m_windowWidth;
    sd.BufferDesc.Height = m_windowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    return D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
        &sd, &m_swapChain, &m_device, nullptr, &m_context);
}

HRESULT GraphicsEngine::CreateRenderTargetView()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr)) return hr;

    hr = m_device->CreateRenderTargetView(pBackBuffer, nullptr, &m_renderTargetView);
    pBackBuffer->Release();
    return hr;
}

HRESULT GraphicsEngine::CreateDepthStencilView()
{
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = m_windowWidth;
    depthDesc.Height = m_windowHeight;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* pDepthStencil = nullptr;
    HRESULT hr = m_device->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil);
    if (FAILED(hr)) return hr;

    hr = m_device->CreateDepthStencilView(pDepthStencil, nullptr, &m_depthStencilView);
    pDepthStencil->Release();
    return hr;
}

void GraphicsEngine::SetViewport()
{
    D3D11_VIEWPORT vp = {};
    vp.Width = (FLOAT)m_windowWidth;
    vp.Height = (FLOAT)m_windowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_context->RSSetViewports(1, &vp);
}
