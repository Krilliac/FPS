#include "GraphicsEngine.h"
#include <DirectXMath.h>

using namespace DirectX;

GraphicsEngine::GraphicsEngine()
    : m_windowWidth(1280)
    , m_windowHeight(720)
{
}

GraphicsEngine::~GraphicsEngine()
{
    Shutdown();
}

HRESULT GraphicsEngine::Initialize(HWND hWnd)
{
    RECT rc; GetClientRect(hWnd, &rc);
    m_windowWidth = rc.right - rc.left;
    m_windowHeight = rc.bottom - rc.top;

    HRESULT hr = CreateDeviceAndSwapChain(hWnd);
    if (FAILED(hr)) return hr;

    hr = CreateRenderTargetView(); if (FAILED(hr)) return hr;
    hr = CreateDepthStencilView(); if (FAILED(hr)) return hr;

    SetViewport();
    return S_OK;
}

void GraphicsEngine::Shutdown()
{
    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
}

void GraphicsEngine::BeginFrame()
{
    const float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(),
        D3D11_CLEAR_DEPTH, 1.0f, 0);
    m_context->OMSetRenderTargets(
        1, m_renderTargetView.GetAddressOf(),
        m_depthStencilView.Get());
}

void GraphicsEngine::EndFrame()
{
    m_swapChain->Present(1, 0);
}

void GraphicsEngine::OnResize(UINT width, UINT height)
{
    if (width == 0 || height == 0) return;
    m_windowWidth = width;
    m_windowHeight = height;

    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    CreateRenderTargetView();
    CreateDepthStencilView();
    SetViewport();
}

HRESULT GraphicsEngine::CreateDeviceAndSwapChain(HWND hWnd)
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    DXGI_SWAP_CHAIN_DESC1 scd = {};
    scd.Width = m_windowWidth;
    scd.Height = m_windowHeight;
    scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.SampleDesc.Count = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 2;
    scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    ComPtr<IDXGIFactory2> factory;
#if defined(_DEBUG)
    UINT factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#else
    UINT factoryFlags = 0;
#endif
    HRESULT hr = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) return hr;

    ComPtr<ID3D11Device> dev;
    ComPtr<ID3D11DeviceContext> ctx;
    hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        flags, levels, _countof(levels),
        D3D11_SDK_VERSION, &dev, nullptr, &ctx);
    if (FAILED(hr)) return hr;

    dev.As(&m_device);
    ctx.As(&m_context);

#ifdef _DEBUG
    // Filter INFO-level D3D11 messages
    {
        ComPtr<ID3D11InfoQueue> infoQueue;
        if (SUCCEEDED(m_device.As(&infoQueue)))
        {
            D3D11_MESSAGE_SEVERITY sev[] = { D3D11_MESSAGE_SEVERITY_INFO };
            D3D11_INFO_QUEUE_FILTER filt = {};
            filt.DenyList.NumSeverities = _countof(sev);
            filt.DenyList.pSeverityList = sev;
            infoQueue->AddStorageFilterEntries(&filt);
            infoQueue->AddRetrievalFilterEntries(&filt);
        }
    }
    // Filter DXGI warning #294
    {
        ComPtr<IDXGIDebug1> dxgiDebug;
        if (SUCCEEDED(DXGIGetDebugInterface1(
            0, IID_PPV_ARGS(&dxgiDebug))))
        {
            ComPtr<IDXGIInfoQueue> dxgiInfo;
            if (SUCCEEDED(dxgiDebug.As(&dxgiInfo)))
            {
                DXGI_INFO_QUEUE_MESSAGE_ID ids[] = { 294 };
                DXGI_INFO_QUEUE_FILTER dxgiFilt = {};
                dxgiFilt.DenyList.NumIDs = _countof(ids);
                dxgiFilt.DenyList.pIDList = ids;
                dxgiInfo->AddStorageFilterEntries(
                    DXGI_DEBUG_ALL, &dxgiFilt);
                dxgiInfo->AddRetrievalFilterEntries(
                    DXGI_DEBUG_ALL, &dxgiFilt);
            }
        }
    }
#endif

    return factory->CreateSwapChainForHwnd(
        m_device.Get(), hWnd, &scd,
        nullptr, nullptr, &m_swapChain);
}

HRESULT GraphicsEngine::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> back;
    HRESULT hr = m_swapChain->GetBuffer(
        0, IID_PPV_ARGS(&back));
    if (FAILED(hr)) return hr;
    return m_device->CreateRenderTargetView(
        back.Get(), nullptr, &m_renderTargetView);
}

HRESULT GraphicsEngine::CreateDepthStencilView()
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_windowWidth;
    desc.Height = m_windowHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ComPtr<ID3D11Texture2D> tex;
    HRESULT hr = m_device->CreateTexture2D(
        &desc, nullptr, &tex);
    if (FAILED(hr)) return hr;
    return m_device->CreateDepthStencilView(
        tex.Get(), nullptr, &m_depthStencilView);
}

void GraphicsEngine::SetViewport()
{
    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = 0; vp.TopLeftY = 0;
    vp.Width = static_cast<float>(m_windowWidth);
    vp.Height = static_cast<float>(m_windowHeight);
    vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &vp);
}
