﻿/**
 * @file EditorApplication.cpp
 * @brief Implementation of the enhanced editor application class
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorApplication.h"
#include "EditorUI.h"  
#include "EditorCrashHandler.h"  
#include "../Utils/SparkConsole.h"
#include <memory>
#include <iostream>
#include <stdexcept>

// Dear ImGui includes
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// DirectX includes
#include <d3d11.h>
#include <dxgi.h>

// Standard includes
#include <iostream>
#include <chrono>
#include <filesystem>
#include <Windows.h>

using Microsoft::WRL::ComPtr;

// External ImGui Win32 message handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace SparkEditor {

// Static instance pointer for message handling
static EditorApplication* s_instance = nullptr;

EditorApplication::EditorApplication()
    : m_startTime(std::chrono::high_resolution_clock::now())
    , m_lastFrameTime(m_startTime)
{
    s_instance = this;
    std::cout << "Enhanced EditorApplication constructed\n";
}

EditorApplication::~EditorApplication()
{
    std::cout << "EditorApplication destructor called\n";
    if (m_isInitialized) {
        Shutdown();
    }
    s_instance = nullptr;
}

bool EditorApplication::Initialize(const EditorConfig& config)
{
    auto& console = Spark::SimpleConsole::GetInstance();
    console.LogInfo("Initializing Enhanced Spark Engine Editor...");
    
    m_config = config;
    m_windowWidth = config.windowWidth;
    m_windowHeight = config.windowHeight;
    
    // Create main editor window
    console.LogInfo("Creating main editor window...");
    if (!CreateMainWindow(config)) {
        console.LogError("Failed to create main editor window");
        return false;
    }
    console.LogSuccess("Main window created successfully (" + std::to_string(m_windowWidth) + "x" + std::to_string(m_windowHeight) + ")");
    
    // Initialize DirectX 11
    console.LogInfo("Initializing DirectX 11...");
    if (!InitializeDirectX()) {
        console.LogError("Failed to initialize DirectX 11");
        return false;
    }
    console.LogSuccess("DirectX 11 initialized successfully");
    
    // Initialize Dear ImGui
    console.LogInfo("Initializing Dear ImGui...");
    if (!InitializeImGui()) {
        console.LogError("Failed to initialize Dear ImGui");
        return false;
    }
    console.LogSuccess("Dear ImGui initialized successfully");
    
    // Initialize EditorUI (this creates all panels including console)
    console.LogInfo("Initializing EditorUI...");
    m_ui = std::make_unique<EditorUI>();
    if (!m_ui->Initialize(config)) {  // Pass config parameter
        console.LogError("Failed to initialize EditorUI");
        return false;
    }
    console.LogSuccess("EditorUI initialized successfully");
    
    m_isInitialized = true;
    m_isRunning = true;
    
    console.LogSuccess("Enhanced Editor initialization complete");
    console.LogInfo("SparkEditor is now ready for use");
    console.LogInfo("All editor operations will be logged to external console");
    
    return true;
}

bool EditorApplication::CreateMainWindow(const EditorConfig& config)
{
    auto& console = Spark::SimpleConsole::GetInstance();
    
    // Register window class
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"SparkEditorWindow";
    
    if (!RegisterClassExW(&wc)) {
        console.LogError("Failed to register window class");
        return false;
    }
    console.LogInfo("Window class registered successfully");
    
    // Create window
    m_hwnd = CreateWindowExW(
        0,                              // dwExStyle
        L"SparkEditorWindow",              // lpClassName 
        L"Spark Engine Editor",                // lpWindowName
        WS_OVERLAPPEDWINDOW,            // dwStyle
        CW_USEDEFAULT,                  // X
        CW_USEDEFAULT,                  // Y  
        config.windowWidth,             // nWidth
        config.windowHeight,            // nHeight
        nullptr,                        // hWndParent
        nullptr,                        // hMenu
        GetModuleHandleW(nullptr),      // hInstance  
        nullptr                         // lpParam
    );
    
    if (!m_hwnd) {
        DWORD error = GetLastError();
        console.LogError("Failed to create window (Error: " + std::to_string(error) + ")");
        return false;
    }
    
    console.LogInfo("Window created successfully");
    
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
    
    console.LogInfo("Window is now visible and active");
    return true;
}

bool EditorApplication::InitializeDirectX()
{
    std::cout << "Initializing DirectX 11...\n";
    
    // Create device and swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = m_windowWidth;
    swapChainDesc.BufferDesc.Height = m_windowHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = m_hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    
    D3D_FEATURE_LEVEL featureLevel;
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // Adapter
        D3D_DRIVER_TYPE_HARDWARE,   // Driver type
        nullptr,                    // Software
        createDeviceFlags,          // Flags
        nullptr,                    // Feature levels
        0,                         // Feature levels count
        D3D11_SDK_VERSION,         // SDK version
        &swapChainDesc,            // Swap chain desc
        &m_swapChain,              // Swap chain
        &m_device,                 // Device
        &featureLevel,             // Feature level
        &m_context                 // Context
    );
    
    if (FAILED(hr)) {
        std::cerr << "Failed to create DirectX device and swap chain\n";
        return false;
    }
    
    // Create render target view
    ComPtr<ID3D11Texture2D> backBuffer;
    hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr)) {
        std::cerr << "Failed to get back buffer\n";
        return false;
    }
    
    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);
    if (FAILED(hr)) {
        std::cerr << "Failed to create render target view\n";
        return false;
    }
    
    std::cout << "DirectX 11 initialized successfully\n";
    return true;
}

bool EditorApplication::InitializeImGui()
{
    std::cout << "Initializing Dear ImGui...\n";
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Note: Docking features require a newer ImGui version
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(m_hwnd)) {
        std::cerr << "Failed to initialize ImGui Win32 backend\n";
        return false;
    }
    
    if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get())) {
        std::cerr << "Failed to initialize ImGui DirectX 11 backend\n";
        return false;
    }
    
    std::cout << "Dear ImGui initialized successfully\n";
    return true;
}

int EditorApplication::Run()
{
    auto& console = Spark::SimpleConsole::GetInstance();
    
    if (!m_isInitialized) {
        console.LogCritical("EditorApplication::Run() called but editor not initialized!");
        return -1;
    }
    
    console.LogInfo("Starting enhanced editor main loop...");
    
    // Main message loop
    MSG msg = {};
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (m_isRunning && msg.message != WM_QUIT) {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Update console (important for external console communication)
        console.Update();
        
        // Process messages
        if (!ProcessMessages()) {
            m_isRunning = false;
            break;
        }
        
        if (!m_isRunning) break;
        
        // Update editor
        Update(deltaTime);
        
        // Render frame
        Render();
        
        // Update performance metrics
        UpdatePerformanceMetrics();
    }
    
    console.LogInfo("Enhanced editor main loop ended");
    return static_cast<int>(msg.wParam);
}

bool EditorApplication::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true;
}

void EditorApplication::Update(float deltaTime)
{
    // Update UI system
    if (m_ui) {
        m_ui->Update(deltaTime);
    }
}

void EditorApplication::Render()
{
    // Clear render target
    float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
    m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), nullptr);
    m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
    
    // Start ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    // Render UI
    if (m_ui) {
        m_ui->Render();
    }
    
    // Render ImGui
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    // Present frame
    m_swapChain->Present(1, 0); // VSync enabled
}

void EditorApplication::UpdatePerformanceMetrics()
{
    static float frameTimeAccumulator = 0.0f;
    static int frameCount = 0;
    static auto lastUpdateTime = std::chrono::high_resolution_clock::now();
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    float frameTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
    m_lastFrameTime = currentTime;
    
    frameTimeAccumulator += frameTime;
    frameCount++;
    
    // Update metrics every 0.5 seconds
    if (std::chrono::duration<float>(currentTime - lastUpdateTime).count() >= 0.5f) {
        m_performanceMetrics.fps = frameCount / frameTimeAccumulator;
        m_performanceMetrics.frameTime = (frameTimeAccumulator / frameCount) * 1000.0f;
        
        frameTimeAccumulator = 0.0f;
        frameCount = 0;
        lastUpdateTime = currentTime;
    }
}

void EditorApplication::RequestExit()
{
    std::cout << "Exit requested\n";
    m_isRunning = false;
}

void EditorApplication::Shutdown()
{
    auto& console = Spark::SimpleConsole::GetInstance();
    console.LogInfo("Shutting down enhanced editor...");
    
    m_isRunning = false;
    
    if (m_ui) {
        console.LogInfo("Shutting down EditorUI...");
        m_ui->Shutdown();
        m_ui.reset();
        console.LogSuccess("EditorUI shutdown complete");
    }
    
    // Cleanup ImGui
    console.LogInfo("Cleaning up Dear ImGui...");
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    console.LogSuccess("Dear ImGui cleanup complete");
    
    // Cleanup DirectX
    console.LogInfo("Cleaning up DirectX 11...");
    m_rtv.Reset();
    m_context.Reset();
    m_device.Reset();
    m_swapChain.Reset();
    console.LogSuccess("DirectX 11 cleanup complete");
    
    // Cleanup window
    if (m_hwnd) {
        console.LogInfo("Destroying main window...");
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
        console.LogSuccess("Main window destroyed");
    }
    
    m_isInitialized = false;
    console.LogSuccess("Enhanced editor shutdown complete");
}

EditorApplication::PerformanceMetrics EditorApplication::GetPerformanceMetrics() const
{
    return m_performanceMetrics;
}

void EditorApplication::OnWindowResize(int width, int height)
{
    if (width <= 0 || height <= 0) return;
    
    m_windowWidth = width;
    m_windowHeight = height;
    
    if (m_swapChain && m_device && m_context) {
        m_rtv.Reset();
        
        HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        if (SUCCEEDED(hr)) {
            ComPtr<ID3D11Texture2D> backBuffer;
            hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
            if (SUCCEEDED(hr)) {
                m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);
            }
        }
    }
}

bool EditorApplication::OnShutdownRequested()
{
    // Use enhanced UI's recovery system to check for unsaved changes
    if (m_ui && m_ui->HasRecoveryData()) {
        // Could show a dialog asking if user wants to save
        // For now, allow shutdown
    }
    
    return true;
}

LRESULT CALLBACK EditorApplication::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Forward messages to ImGui
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;
    
    // Get application instance
    EditorApplication* app = s_instance;
    if (!app && msg == WM_CREATE) {
        CREATESTRUCTW* createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        app = static_cast<EditorApplication*>(createStruct->lpCreateParams);
    }
    
    switch (msg) {
        case WM_SIZE:
            if (app && wParam != SIZE_MINIMIZED) {
                app->OnWindowResize(LOWORD(lParam), HIWORD(lParam));
            }
            return 0;
        
        case WM_CLOSE:
            if (app && app->OnShutdownRequested()) {
                app->RequestExit();
            }
            return 0;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

} // namespace SparkEditor
