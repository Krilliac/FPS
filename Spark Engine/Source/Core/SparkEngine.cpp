#include "framework.h"
#include "SparkEngine.h"
#include "../Utils/Assert.h"
#include "../Utils/Profiler.h"

#include <windows.h>
#include <iostream>
#include <memory>
#include <exception>

#include "../Game/SparkEngineGame.h"

// Global variables
constexpr int MAX_LOADSTRING = 100;
HINSTANCE g_hInst = nullptr;
WCHAR g_szTitle[MAX_LOADSTRING] = L"SparkEngine - Complete Unified System";
WCHAR g_szClass[MAX_LOADSTRING] = L"SparkEngineCompleteWindowClass";
HWND g_hWnd = nullptr;

// Forward declarations
ATOM RegisterWindowClass(HINSTANCE hInstance);
BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// System initialization wrapper with comprehensive error handling
template<typename T, typename... Args>
bool SafeInitializeSystem(std::unique_ptr<T>& system, const std::string& systemName, Args&&... args) {
    try {
        if (!system) {
            system = std::make_unique<T>();
        }
        
        if (!system) {
            std::cerr << "CRITICAL: Failed to create " << systemName << " - out of memory" << std::endl;
            return false;
        }

        bool result = false;
        
        // Handle different initialization signatures
        if constexpr (sizeof...(args) == 0) {
            result = system->Initialize();
        } else {
            result = system->Initialize(std::forward<Args>(args)...);
        }

        if (result) {
            std::cout << "âœ“ " << systemName << " initialized successfully" << std::endl;
        } else {
            std::cerr << "âœ— " << systemName << " initialization failed" << std::endl;
        }
        
        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION in " << systemName << " initialization: " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cerr << "UNKNOWN EXCEPTION in " << systemName << " initialization" << std::endl;
        return false;
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR lpCmdLine,
                     _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    std::cout << "Starting SparkEngine Complete Unified System..." << std::endl;

    // Initialize profiler early
    SparkEngine::Profiler::Get().SetEnabled(true);
    SparkEngine::Profiler::Get().BeginFrame();

    // Set up comprehensive exception handling
    std::set_terminate([]() {
        try {
            auto p = std::current_exception();
            if (p) {
                std::rethrow_exception(p);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "FATAL TERMINATION: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "FATAL TERMINATION: Unknown exception" << std::endl;
        }
        
        MessageBoxA(nullptr, 
                   "SparkEngine encountered a fatal error and must close.\nCheck the console for details.", 
                   "SparkEngine Fatal Error", 
                   MB_OK | MB_ICONERROR);
        std::abort();
    });

    g_hInst = hInstance;

    // Load string resources safely
    LoadStringW(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPARKENGINE, g_szClass, MAX_LOADSTRING);

    // Register window class
    if (!RegisterWindowClass(hInstance)) {
        MessageBoxA(nullptr, "Failed to register window class", "Initialization Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Create main window
    if (!CreateMainWindow(hInstance, nCmdShow)) {
        MessageBoxA(nullptr, "Failed to create main window", "Initialization Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Initialize the complete unified engine system
    std::unique_ptr<SparkEngine::SparkEngineGame> unifiedEngine;
    
    try {
        PROFILE_SCOPE("Engine Initialization");
        
        unifiedEngine = std::make_unique<SparkEngine::SparkEngineGame>();
        
        if (!SafeInitializeSystem(unifiedEngine, "Complete SparkEngine System", g_hWnd, 1280, 720)) {
            MessageBoxA(g_hWnd, 
                       "Failed to initialize the complete SparkEngine system.\nCheck console for details.", 
                       "System Initialization Error", 
                       MB_OK | MB_ICONERROR);
            return -1;
        }
        
        std::cout << "ðŸš€ SparkEngine Complete System initialized successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::string error = "Critical exception during engine initialization: " + std::string(e.what());
        std::cerr << error << std::endl;
        MessageBoxA(g_hWnd, error.c_str(), "Critical Initialization Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Main application loop with comprehensive error handling
    MSG msg = {};
    auto lastTime = std::chrono::high_resolution_clock::now();
    bool running = true;
    int frameCount = 0;
    
    std::cout << "Entering main application loop..." << std::endl;

    while (running && msg.message != WM_QUIT) {
        PROFILE_SCOPE("Main Loop");
        
        // Process Windows messages
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!running) break;

        try {
            PROFILE_SCOPE("Frame Processing");
            
            // Calculate frame time with smoothing
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            // Cap delta time to prevent simulation instability
            deltaTime = std::min(deltaTime, 0.033f); // Minimum 30 FPS
            deltaTime = std::max(deltaTime, 0.001f); // Maximum 1000 FPS

            // Update and render the complete system
            if (unifiedEngine && unifiedEngine->IsRunning()) {
                {
                    PROFILE_SCOPE("Engine Update");
                    unifiedEngine->Update(deltaTime);
                }
                
                {
                    PROFILE_SCOPE("Engine Render");
                    unifiedEngine->Render();
                }
                
                frameCount++;
                
                // Performance monitoring
                if (frameCount % 300 == 0) { // Every ~5 seconds at 60 FPS
                    const auto& stats = SparkEngine::Profiler::Get().GetFrameSamples();
                    std::cout << "Performance update - Frame " << frameCount 
                              << ", Average frame time: " << SparkEngine::Profiler::Get().GetAverageFrameTime() * 1000.0 
                              << "ms" << std::endl;
                }
            } else {
                std::cout << "Engine stopped running, exiting main loop..." << std::endl;
                running = false;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Exception in main loop (Frame " << frameCount << "): " << e.what() << std::endl;
            
            // Try to recover once, then exit
            static bool recoveryAttempted = false;
            if (!recoveryAttempted) {
                std::cout << "Attempting recovery..." << std::endl;
                recoveryAttempted = true;
                continue;
            } else {
                std::cerr << "Recovery failed, shutting down..." << std::endl;
                MessageBoxA(g_hWnd, ("Runtime error: " + std::string(e.what())).c_str(), 
                           "Runtime Error", MB_OK | MB_ICONERROR);
                running = false;
            }
        }
        catch (...) {
            std::cerr << "Unknown exception in main loop (Frame " << frameCount << ")" << std::endl;
            MessageBoxA(g_hWnd, "Unknown runtime error occurred", "Runtime Error", MB_OK | MB_ICONERROR);
            running = false;
        }
        
        // End frame profiling
        SparkEngine::Profiler::Get().EndFrame();
        SparkEngine::Profiler::Get().BeginFrame();
    }

    // Comprehensive shutdown sequence
    std::cout << "Beginning shutdown sequence..." << std::endl;
    
    try {
        PROFILE_SCOPE("Engine Shutdown");
        
        if (unifiedEngine) {
            unifiedEngine->Shutdown();
            unifiedEngine.reset();
        }
        
        SparkEngine::Profiler::Get().EndFrame();
        
        std::cout << "SparkEngine Complete System shutdown successfully" << std::endl;
        std::cout << "Total frames processed: " << frameCount << std::endl;
        
        // Print final performance summary
        const auto& finalStats = SparkEngine::Profiler::Get().GetFrameSamples();
        if (!finalStats.empty()) {
            std::cout << "\nFinal Performance Summary:" << std::endl;
            for (const auto& sample : finalStats) {
                if (sample.callCount > 0) {
                    std::cout << "  " << sample.name << ": " 
                              << (sample.duration * 1000.0) << "ms avg, "
                              << sample.callCount << " calls" << std::endl;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception during shutdown: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception during shutdown" << std::endl;
    }

    return static_cast<int>(msg.wParam);
}

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPARKENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPARKENGINE);
    wcex.lpszClassName = g_szClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
    g_hWnd = CreateWindowW(g_szClass, g_szTitle, WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd) {
        DWORD error = GetLastError();
        std::cerr << "CreateWindow failed with error: 0x" << std::hex << error << std::dec << std::endl;
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        // Forward resize events to the engine (will be handled in SparkEngineGame)
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEWHEEL:
        // Forward input events to the engine
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
