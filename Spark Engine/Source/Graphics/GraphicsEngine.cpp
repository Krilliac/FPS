// GraphicsEngine.cpp
#include "GraphicsEngine.h"
#include "Utils/Assert.h"
#include "../Utils/SparkConsole.h"

#include <DirectXMath.h>
#include <cstdio>
#include <iostream>
#include <chrono>

using namespace DirectX;

// **SIMPLE: Use SimpleConsole for logging - always works**
#define LOG_TO_CONSOLE_IMMEDIATE(wmsg, wtype) \
    do { \
        std::wstring wstr = wmsg; \
        std::wstring wtypestr = wtype; \
        std::string msg(wstr.begin(), wstr.end()); \
        std::string type(wtypestr.begin(), wtypestr.end()); \
        Spark::SimpleConsole::GetInstance().Log(msg, type); \
    } while(0)

#define LOG_TO_CONSOLE_RATE_LIMITED(wmsg, wtype) \
    do { \
        static auto lastLogTime = std::chrono::steady_clock::now(); \
        static int logCounter = 0; \
        auto now = std::chrono::steady_clock::now(); \
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastLogTime).count(); \
        if (elapsed >= 5 || logCounter < 2) { \
            std::wstring wstr = wmsg; \
            std::wstring wtypestr = wtype; \
            std::string msg(wstr.begin(), wstr.end()); \
            std::string type(wtypestr.begin(), wtypestr.end()); \
            Spark::SimpleConsole::GetInstance().Log(msg, type); \
            if (elapsed >= 5) { \
                lastLogTime = now; \
                logCounter = 0; \
            } else { \
                logCounter++; \
            } \
        } \
    } while(0)

// Use rate-limited logging for most messages, immediate for critical ones
#define LOG_TO_CONSOLE(wmsg, wtype) LOG_TO_CONSOLE_RATE_LIMITED(wmsg, wtype)

// Constructor / destructor
GraphicsEngine::GraphicsEngine()
    : m_windowWidth(1280)
    , m_windowHeight(720)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine constructed.", L"INFO");
}

GraphicsEngine::~GraphicsEngine()
{
    LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine destructor called.", L"INFO");
    Shutdown();
}

// Initialize device, swap chain, RTV/DSV, debug filters
HRESULT GraphicsEngine::Initialize(HWND hWnd)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine::Initialize started.", L"INFO");
    ASSERT(hWnd != nullptr);
    RECT rc; GetClientRect(hWnd, &rc);
    m_windowWidth = rc.right - rc.left;
    m_windowHeight = rc.bottom - rc.top;
    
    std::wstring sizeMsg = L"Window size: " + std::to_wstring(m_windowWidth) + L"x" + std::to_wstring(m_windowHeight);
    LOG_TO_CONSOLE_IMMEDIATE(sizeMsg, L"INFO");
    ASSERT_MSG(m_windowWidth > 0 && m_windowHeight > 0, "Invalid window size");
    
    HRESULT hr = CreateDeviceAndSwapChain(hWnd);
    ASSERT_MSG(SUCCEEDED(hr), "CreateDeviceAndSwapChain failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"CreateDeviceAndSwapChain failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }
    
    hr = CreateRenderTargetView();
    ASSERT_MSG(SUCCEEDED(hr), "CreateRenderTargetView failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"CreateRenderTargetView failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }
    
    hr = CreateDepthStencilView();
    ASSERT_MSG(SUCCEEDED(hr), "CreateDepthStencilView failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"CreateDepthStencilView failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return hr;
    }
    
    SetViewport();
    LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine initialization complete - rendering ready.", L"SUCCESS");
    return S_OK;
}

// Release COM resources
void GraphicsEngine::Shutdown()
{
    LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine::Shutdown called.", L"INFO");
    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
    LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine shutdown complete.", L"INFO");
}

// Clear and bind render targets
void GraphicsEngine::BeginFrame()
{
    // **FIXED: Remove per-frame logging completely**
    static bool firstFrame = true;
    if (firstFrame) {
        LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine::BeginFrame - First frame started", L"INFO");
        firstFrame = false;
    }
    
    ASSERT(m_context && m_renderTargetView && m_depthStencilView);
    const float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

// Present back buffer
void GraphicsEngine::EndFrame()
{
    // **FIXED: Remove per-frame logging completely**
    static bool firstFrame = true;
    if (firstFrame) {
        LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine::EndFrame - First frame presented", L"INFO");
        firstFrame = false;
    }
    
    ASSERT(m_swapChain);
    m_swapChain->Present(1, 0);
}

// Handle window resize
void GraphicsEngine::OnResize(UINT width, UINT height)
{
    // Only log if the size actually changes significantly
    static UINT lastWidth = 0, lastHeight = 0;
    bool sizeChanged = (width != lastWidth || height != lastHeight);
    if (sizeChanged && (width > 0 && height > 0)) {
        std::wstring resizeMsg = L"GraphicsEngine::OnResize " + std::to_wstring(width) + L"x" + std::to_wstring(height);
        LOG_TO_CONSOLE_IMMEDIATE(resizeMsg, L"INFO");
        lastWidth = width;
        lastHeight = height;
    }
    
    if (width == 0 || height == 0) return;
    m_windowWidth = width;
    m_windowHeight = height;
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    ASSERT_MSG(SUCCEEDED(hr), "ResizeBuffers failed");
    if (FAILED(hr)) {
        std::wstring errorMsg = L"ResizeBuffers failed with HR=0x" + std::to_wstring(hr);
        LOG_TO_CONSOLE_IMMEDIATE(errorMsg, L"ERROR");
        return;
    }
    CreateRenderTargetView();
    CreateDepthStencilView();
    SetViewport();
    
    if (sizeChanged) {
        LOG_TO_CONSOLE_IMMEDIATE(L"GraphicsEngine resize complete.", L"INFO");
    }
}

// Create D3D11 device, context, and swap chain
HRESULT GraphicsEngine::CreateDeviceAndSwapChain(HWND hWnd)
{
    ASSERT(hWnd != nullptr);

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

    UINT factoryFlags = 0;
#ifdef _DEBUG
    factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

    ComPtr<IDXGIFactory2> factory;
    HRESULT hr = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));
    ASSERT_MSG(SUCCEEDED(hr), "CreateDXGIFactory2 failed");
    if (FAILED(hr)) return hr;

    ComPtr<ID3D11Device> dev;
    ComPtr<ID3D11DeviceContext> ctx;
    hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        flags, levels, _countof(levels),
        D3D11_SDK_VERSION, &dev, nullptr, &ctx);
    ASSERT_MSG(SUCCEEDED(hr), "D3D11CreateDevice failed");
    if (FAILED(hr)) return hr;

    hr = dev.As(&m_device);
    ASSERT_MSG(SUCCEEDED(hr), "Query ID3D11Device1 failed");
    hr = ctx.As(&m_context);
    ASSERT_MSG(SUCCEEDED(hr), "Query ID3D11DeviceContext1 failed");

#ifdef _DEBUG
    // D3D11 info queue filter (optional, omitted for brevity)
    // DXGI debug filter (optional)
#endif

    hr = factory->CreateSwapChainForHwnd(
        m_device.Get(), hWnd, &scd, nullptr, nullptr, &m_swapChain);
    ASSERT_MSG(SUCCEEDED(hr), "CreateSwapChainForHwnd failed");
    return hr;
}

// Create render target view from swap chain back buffer
HRESULT GraphicsEngine::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> back;
    HRESULT hr = m_swapChain->GetBuffer(
        0, IID_PPV_ARGS(&back));
    ASSERT_MSG(SUCCEEDED(hr), "SwapChain GetBuffer failed");
    if (FAILED(hr)) return hr;

    hr = m_device->CreateRenderTargetView(
        back.Get(), nullptr, &m_renderTargetView);
    ASSERT_MSG(SUCCEEDED(hr), "CreateRenderTargetView failed");
    return hr;
}

// Create depth-stencil view
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
    ASSERT_MSG(SUCCEEDED(hr), "CreateTexture2D for DSV failed");
    if (FAILED(hr)) return hr;

    hr = m_device->CreateDepthStencilView(
        tex.Get(), nullptr, &m_depthStencilView);
    ASSERT_MSG(SUCCEEDED(hr), "CreateDepthStencilView failed");
    return hr;
}

// Set viewport to cover entire render target
void GraphicsEngine::SetViewport()
{
    ASSERT(m_context);

    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = 0; vp.TopLeftY = 0;
    vp.Width = static_cast<float>(m_windowWidth);
    vp.Height = static_cast<float>(m_windowHeight);
    vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &vp);
}