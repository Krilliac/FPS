/**
 * @file EditorApplication.cpp
 * @brief Implementation of the core editor application class
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorApplication.h"
#include "EditorUI.h"
#include "../SceneSystem/SceneManager.h"
#include "../AssetBrowser/AssetDatabase.h"
#include "../Communication/EngineInterface.h"
#include "ProjectManager.h"
#include "EditorTheme.h"

// Dear ImGui includes
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// DirectX includes
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

// Standard includes
#include <iostream>
#include <chrono>
#include <algorithm>

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
    std::cout << "EditorApplication constructed\n";
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
    std::cout << "Initializing Spark Engine Editor...\n";
    m_config = config;
    
    // Create main window
    if (!CreateMainWindow(config)) {
        std::cerr << "Failed to create main window\n";
        return false;
    }
    
    // Initialize DirectX 11
    if (!InitializeDirectX()) {
        std::cerr << "Failed to initialize DirectX 11\n";
        return false;
    }
    
    // Initialize Dear ImGui
    if (!InitializeImGui()) {
        std::cerr << "Failed to initialize Dear ImGui\n";
        return false;
    }
    
    // Initialize editor subsystems
    if (!InitializeSubsystems(config)) {
        std::cerr << "Failed to initialize editor subsystems\n";
        return false;
    }
    
    m_isInitialized = true;
    std::cout << "Editor initialization complete\n";
    return true;
}

bool EditorApplication::CreateMainWindow(const EditorConfig& config)
{
    std::cout << "Creating main editor window...\n";
    
    // Register window class
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = config.hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"SparkEditorWindow";
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    
    if (!RegisterClassExW(&wc)) {
        std::cerr << "Failed to register window class\n";
        return false;
    }
    
    // Calculate window size including borders
    RECT windowRect = { 0, 0, config.windowWidth, config.windowHeight };
    AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, 0);
    
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;
    
    // Center window on screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;
    
    // Create window
    m_hwnd = CreateWindowExW(
        0,                              // Extended style
        L"SparkEditorWindow",           // Class name
        L"Spark Engine Editor",         // Window title
        WS_OVERLAPPEDWINDOW,           // Style
        posX, posY,                    // Position
        windowWidth, windowHeight,      // Size
        nullptr,                       // Parent window
        nullptr,                       // Menu
        config.hInstance,              // Instance
        this                          // Additional data
    );
    
    if (!m_hwnd) {
        std::cerr << "Failed to create window\n";
        return false;
    }
    
    // Show and update window
    ShowWindow(m_hwnd, config.maximizeOnStart ? SW_SHOWMAXIMIZED : config.nCmdShow);
    UpdateWindow(m_hwnd);
    
    // Get actual client area size
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    m_windowWidth = clientRect.right - clientRect.left;
    m_windowHeight = clientRect.bottom - clientRect.top;
    
    std::cout << "Main window created successfully (" << m_windowWidth << "x" << m_windowHeight << ")\n";
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
        std::cerr << "Failed to create DirectX device and swap chain: 0x" << std::hex << hr << std::dec << "\n";
        return false;
    }
    
    // Create render target view
    ComPtr<ID3D11Texture2D> backBuffer;
    hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr)) {
        std::cerr << "Failed to get back buffer: 0x" << std::hex << hr << std::dec << "\n";
        return false;
    }
    
    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);
    if (FAILED(hr)) {
        std::cerr << "Failed to create render target view: 0x" << std::hex << hr << std::dec << "\n";
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
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Enable keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    
    // Try to enable docking if available (may not be in all ImGui versions)
    try {
        io.ConfigFlags |= 0x00000020;  // ImGuiConfigFlags_DockingEnable equivalent
    } catch (...) {
        std::cout << "Docking not available in this ImGui version\n";
    }
    
    // Setup Dear ImGui style
    EditorTheme::ApplyTheme(m_config.themeName);
    
    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(m_hwnd)) {
        std::cerr << "Failed to initialize ImGui Win32 backend\n";
        return false;
    }
    
    if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get())) {
        std::cerr << "Failed to initialize ImGui DirectX 11 backend\n";
        return false;
    }
    
    // Load fonts
    io.Fonts->AddFontDefault();
    
    // TODO: Load custom fonts for the editor
    // ImFont* font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto-Medium.ttf", 16.0f);
    // io.FontDefault = font;
    
    std::cout << "Dear ImGui initialized successfully\n";
    return true;
}

bool EditorApplication::InitializeSubsystems(const EditorConfig& config)
{
    std::cout << "Initializing editor subsystems...\n";
    
    try {
        // Initialize UI system
        m_ui = std::make_unique<EditorUI>();
        if (!m_ui->Initialize()) {
            std::cerr << "Failed to initialize UI system\n";
            return false;
        }
        
        // Initialize scene manager
        m_sceneManager = std::make_unique<SceneManager>();
        if (!m_sceneManager->Initialize()) {
            std::cerr << "Failed to initialize scene manager\n";
            return false;
        }
        
        // Initialize asset database
        m_assetDatabase = std::make_unique<AssetDatabase>();
        if (!m_assetDatabase->Initialize()) {
            std::cerr << "Failed to initialize asset database\n";
            return false;
        }
        
        // Initialize project manager
        m_projectManager = std::make_unique<ProjectManager>();
        if (!m_projectManager->Initialize()) {
            std::cerr << "Failed to initialize project manager\n";
            return false;
        }
        
        // Initialize engine interface
        if (config.enableEngineConnection) {
            m_engineInterface = std::make_unique<EngineInterface>();
            if (!m_engineInterface->Initialize()) {
                std::cout << "Warning: Failed to initialize engine interface (engine may not be running)\n";
            }
        }
        
        // Load project if specified
        if (!config.projectPath.empty()) {
            m_currentProjectPath = config.projectPath;
            // TODO: Load project
        }
        
        // Load scene if specified
        if (!config.scenePath.empty()) {
            // TODO: Load scene
        }
        
        std::cout << "Editor subsystems initialized successfully\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception during subsystem initialization: " << e.what() << "\n";
        return false;
    }
}

int EditorApplication::Run()
{
    std::cout << "Starting editor main loop...\n";
    m_isRunning = true;
    
    while (m_isRunning) {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
        m_lastFrameTime = currentTime;
        
        // Process Windows messages
        if (!ProcessMessages()) {
            break;
        }
        
        // Update subsystems
        Update(deltaTime);
        
        // Render frame
        Render();
        
        // Update performance metrics
        UpdatePerformanceMetrics();
    }
    
    std::cout << "Editor main loop finished\n";
    return 0;
}

bool EditorApplication::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return true;
}

void EditorApplication::Update(float deltaTime)
{
    // Update UI system
    if (m_ui) {
        m_ui->Update(deltaTime);
    }
    
    // Update scene manager
    if (m_sceneManager) {
        m_sceneManager->Update(deltaTime);
    }
    
    // Update asset database
    if (m_assetDatabase) {
        m_assetDatabase->Update(deltaTime);
    }
    
    // Update project manager (if needed)
    // ProjectManager doesn't need per-frame updates
    
    // Update engine interface
    if (m_engineInterface) {
        m_engineInterface->Update(deltaTime);
    }
}

void EditorApplication::Render()
{
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
    
    // Clear render target
    const float clearColor[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
    m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), nullptr);
    m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
    
    // Render ImGui draw data
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows if available
#ifdef IMGUI_HAS_VIEWPORT
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
#endif
    
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
    
    frameTimeAccumulator += frameTime;
    frameCount++;
    
    // Update metrics every 0.5 seconds
    if (std::chrono::duration<float>(currentTime - lastUpdateTime).count() >= 0.5f) {
        m_performanceMetrics.fps = frameCount / frameTimeAccumulator;
        m_performanceMetrics.frameTime = (frameTimeAccumulator / frameCount) * 1000.0f; // Convert to ms
        
        frameTimeAccumulator = 0.0f;
        frameCount = 0;
        lastUpdateTime = currentTime;
        
        // TODO: Update memory usage metrics
        m_performanceMetrics.memoryUsage = 0; // Placeholder
        m_performanceMetrics.gpuMemoryUsage = 0; // Placeholder
    }
}

void EditorApplication::RequestExit()
{
    std::cout << "Exit requested\n";
    m_isRunning = false;
}

void EditorApplication::Shutdown()
{
    std::cout << "Shutting down editor...\n";
    
    m_isRunning = false;
    
    // Shutdown subsystems in reverse order
    m_engineInterface.reset();
    m_projectManager.reset();
    m_assetDatabase.reset();
    m_sceneManager.reset();
    m_ui.reset();
    
    // Cleanup ImGui
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    // Cleanup DirectX
    m_rtv.Reset();
    m_swapChain.Reset();
    m_context.Reset();
    m_device.Reset();
    
    // Destroy window
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
    
    m_isInitialized = false;
    std::cout << "Editor shutdown complete\n";
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
        // Release render target view
        m_rtv.Reset();
        
        // Resize swap chain buffers
        HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        if (SUCCEEDED(hr)) {
            // Recreate render target view
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
    // TODO: Check for unsaved changes and prompt user
    // For now, always allow shutdown
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
        CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
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
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

} // namespace SparkEditor