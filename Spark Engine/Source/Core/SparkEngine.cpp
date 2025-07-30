// SparkEngine.cpp – unified entry point with Win32 boilerplate + classic subsystems
// ===================================================================================

#include "framework.h"
#include "SparkEngine.h"
#include "Utils/Assert.h"             // <-- custom assert

#include <Windows.h>
#include <memory>
#include <DirectXMath.h>
#include <cstdio>                     // for swprintf_s

#include "../Graphics/GraphicsEngine.h"
#include "../Game/Game.h"
#include "../Input/InputManager.h"
#include "../Utils/Timer.h"
#include "../Game/Console.h"
#include "Utils/CrashHandler.h"

// -----------------------------------------------------------------------------
// Globals & constants
// -----------------------------------------------------------------------------
constexpr int MAX_LOADSTRING = 100;

HINSTANCE                          g_hInst;
WCHAR                              g_szTitle[MAX_LOADSTRING];
WCHAR                              g_szClass[MAX_LOADSTRING];

// Classic subsystems
std::unique_ptr<GraphicsEngine>    g_graphics;
std::unique_ptr<Game>              g_game;
std::unique_ptr<InputManager>      g_input;
std::unique_ptr<Timer>             g_timer;
Console                            g_console;

// Win32 forward declarations
ATOM                MyRegisterClass(HINSTANCE);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// ===================================================================================
//                                    wWinMain
// ===================================================================================
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR,
    _In_ int nCmdShow)
{
    ASSERT(hInstance != nullptr);

    // 1. Crash handler
    CrashConfig crashCfg{};
    crashCfg.dumpPrefix = L"SparkCrash";
    crashCfg.uploadURL = "https://crash.placeholder.com/upload";
    crashCfg.captureScreenshot = true;
    crashCfg.captureSystemInfo = true;
    crashCfg.captureAllThreads = true;
    crashCfg.zipBeforeUpload = true;
    crashCfg.triggerCrashOnAssert = false;  // NEW: Don't crash on assertions during development
    InstallCrashHandler(crashCfg);

    // 2. Class & window title
    ASSERT(MAX_LOADSTRING <= _countof(g_szClass) && MAX_LOADSTRING <= _countof(g_szTitle));
    wcscpy_s(g_szClass, MAX_LOADSTRING, L"SparkEngineWindowClass");
    wcscpy_s(g_szTitle, MAX_LOADSTRING, L"Spark Engine");

    // 3. Register window class
    ATOM cls = MyRegisterClass(hInstance);
    ASSERT_MSG(cls != 0, "MyRegisterClass failed");
    if (cls == 0)
    {
        MessageBoxW(nullptr, L"RegisterClassExW failed", L"Fatal Error", MB_ICONERROR);
        return -1;
    }

    // 4. Create window & init subsystems
    if (!InitInstance(hInstance, nCmdShow))
        return -1;

    // 5. Message loop + tick
    HACCEL accel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SparkEngine));
    MSG msg = {};
    g_timer = std::make_unique<Timer>();
    ASSERT(g_timer);

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // Console first
            if (msg.message == WM_CHAR &&
                g_console.HandleChar(static_cast<wchar_t>(msg.wParam)))
                continue;
            if (msg.message == WM_KEYDOWN &&
                g_console.HandleKeyDown(msg.wParam))
                continue;

            if (!TranslateAccelerator(msg.hwnd, accel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            float dt = g_timer->GetDeltaTime();

            if (g_input)                      g_input->Update();
            if (g_game && !g_console.IsVisible()) g_game->Update(dt);

            g_graphics->BeginFrame();
            if (g_game)        g_game->Render();
            if (g_console.IsVisible())
                g_console.Render(g_graphics->GetContext());
            g_graphics->EndFrame();
        }
    }

    return static_cast<int>(msg.wParam);
}

// ===================================================================================
//                       Win32 boilerplate
// ===================================================================================
ATOM MyRegisterClass(HINSTANCE hInst)
{
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SparkEngine));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCEW(IDC_SparkEngine);
    wc.lpszClassName = g_szClass;
    wc.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));

    ATOM result = RegisterClassExW(&wc);
    ASSERT_MSG(result != 0, "RegisterClassExW returned zero");
    return result;
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    ASSERT(hInst != nullptr);
    g_hInst = hInst;

    // Create window
    HWND hWnd = CreateWindowW(
        g_szClass, g_szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1280, 720,
        nullptr, nullptr, hInst, nullptr);

    if (!hWnd)
    {
        DWORD err = GetLastError();
        wchar_t buf[256];
        swprintf_s(buf, L"CreateWindowW failed (0x%08X)", static_cast<unsigned>(err));
        MessageBoxW(nullptr, buf, L"Fatal Error", MB_ICONERROR);
        return FALSE;
    }

    // 1. Initialize Console
    g_console.Initialize(1280, 720);

    // 2. Initialize Graphics
    g_graphics = std::make_unique<GraphicsEngine>();
    ASSERT(g_graphics);
    if (FAILED(g_graphics->Initialize(hWnd)))
    {
        MessageBoxW(hWnd, L"Graphics init failed", L"Fatal", MB_ICONERROR);
        return FALSE;
    }

    // 3. Initialize Input
    g_input = std::make_unique<InputManager>();
    ASSERT(g_input);
    g_input->Initialize(hWnd);

    // 4. Initialize Game
    g_game = std::make_unique<Game>();
    ASSERT(g_game);
    {
        HRESULT hr = g_game->Initialize(g_graphics.get(), g_input.get());
        ASSERT_MSG(SUCCEEDED(hr), "Game::Initialize failed");
        if (FAILED(hr))
        {
            MessageBoxW(nullptr, L"Game initialization failed", L"Fatal Error", MB_ICONERROR);
            return FALSE;
        }
    }

    // 5. Show window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

// ===================================================================================
//                          Window procedure
// ===================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_OEM_3)
        {
            g_console.Toggle();
            return 0;
        }
        if (g_input) g_input->HandleMessage(msg, wParam, lParam);
        break;

    case WM_KEYUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN: case WM_LBUTTONUP:
    case WM_RBUTTONDOWN: case WM_RBUTTONUP:
        if (g_input) g_input->HandleMessage(msg, wParam, lParam);
        break;

    case WM_SIZE:
        if (g_graphics)
            g_graphics->OnResize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT_PTR CALLBACK About(HWND hDlg, UINT msg, WPARAM wParam, LPARAM)
{
    if (msg == WM_COMMAND &&
        (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL))
    {
        EndDialog(hDlg, LOWORD(wParam));
        return TRUE;
    }
    return FALSE;
}