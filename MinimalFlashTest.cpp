/**
 * @file MinimalFlashTest.cpp
 * @brief Minimal test to isolate flashing root cause
 * @author Spark Engine Team
 * @date 2025
 */

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <DirectXMath.h>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace DirectX;

// Global variables
HWND g_hWnd = nullptr;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
IDXGISwapChain1* g_swapChain = nullptr;
ID3D11RenderTargetView* g_renderTargetView = nullptr;
bool g_running = true;

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            g_running = false;
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                g_running = false;
                PostQuitMessage(0);
            }
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool InitWindow() {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"MinimalFlashTest";
    
    if (!RegisterClassEx(&wc)) {
        std::wcout << L"Failed to register window class" << std::endl;
        return false;
    }
    
    g_hWnd = CreateWindowEx(
        0,
        L"MinimalFlashTest",
        L"Minimal Flash Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720,
        nullptr, nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    
    if (!g_hWnd) {
        std::wcout << L"Failed to create window" << std::endl;
        return false;
    }
    
    ShowWindow(g_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(g_hWnd);
    
    return true;
}

bool InitD3D() {
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;
    
    // Create device and context
    UINT flags = 0;
    #ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        nullptr, 0,
        D3D11_SDK_VERSION,
        &g_device,
        &featureLevel,
        &g_context
    );
    
    if (FAILED(hr)) {
        std::wcout << L"Failed to create D3D11 device: 0x" << std::hex << hr << std::endl;
        return false;
    }
    
    // Create swap chain
    IDXGIFactory2* factory = nullptr;
    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)&factory);
    if (FAILED(hr)) {
        std::wcout << L"Failed to create DXGI factory: 0x" << std::hex << hr << std::endl;
        return false;
    }
    
    DXGI_SWAP_CHAIN_DESC1 scd = {};
    scd.Width = width;
    scd.Height = height;
    scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 2;
    scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;  // Stable swap effect
    scd.Flags = 0;
    
    hr = factory->CreateSwapChainForHwnd(
        g_device,
        g_hWnd,
        &scd,
        nullptr,
        nullptr,
        &g_swapChain
    );
    
    factory->Release();
    
    if (FAILED(hr)) {
        std::wcout << L"Failed to create swap chain: 0x" << std::hex << hr << std::endl;
        return false;
    }
    
    // Create render target view
    ID3D11Texture2D* backBuffer = nullptr;
    hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr)) {
        std::wcout << L"Failed to get back buffer: 0x" << std::hex << hr << std::endl;
        return false;
    }
    
    hr = g_device->CreateRenderTargetView(backBuffer, nullptr, &g_renderTargetView);
    backBuffer->Release();
    
    if (FAILED(hr)) {
        std::wcout << L"Failed to create render target view: 0x" << std::hex << hr << std::endl;
        return false;
    }
    
    // Set viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_context->RSSetViewports(1, &vp);
    
    std::wcout << L"D3D11 initialized successfully" << std::endl;
    return true;
}

void Cleanup() {
    if (g_renderTargetView) g_renderTargetView->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
}

void Render() {
    // **TEST 1: Just clear to blue**
    float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f }; // Dark blue
    
    // Single clear operation
    g_context->ClearRenderTargetView(g_renderTargetView, clearColor);
    
    // Single present operation
    HRESULT hr = g_swapChain->Present(1, 0); // VSync on
    
    if (FAILED(hr)) {
        std::wcout << L"Present failed: 0x" << std::hex << hr << std::endl;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::wcout << L"=== MINIMAL FLASH TEST ===" << std::endl;
    std::wcout << L"Testing absolute minimal DirectX rendering" << std::endl;
    std::wcout << L"If this flashes, the issue is in DirectX setup" << std::endl;
    std::wcout << L"If this is stable, the issue is in engine systems" << std::endl;
    std::wcout << L"Press ESC to exit" << std::endl;
    std::wcout << L"=========================" << std::endl;
    
    if (!InitWindow()) {
        std::wcout << L"Window initialization failed" << std::endl;
        return -1;
    }
    
    if (!InitD3D()) {
        std::wcout << L"D3D initialization failed" << std::endl;
        return -1;
    }
    
    std::wcout << L"Starting render loop..." << std::endl;
    
    MSG msg = {};
    while (g_running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        if (g_running) {
            Render();
        }
    }
    
    std::wcout << L"Cleaning up..." << std::endl;
    Cleanup();
    
    std::wcout << L"Test completed. Did you see any flashing?" << std::endl;
    return 0;
}