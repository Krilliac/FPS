// ===================================================================================
// FPS.cpp – unified entry point with Win32 boilerplate + Spark Engine integration
// ===================================================================================

#include "framework.h"
#include "FPS.h"

#include <Windows.h>
#include <memory>
#include <DirectXMath.h>

#include "..\Graphics\GraphicsEngine.h"
#include "..\Game\Game.h"
#include "..\Input\InputManager.h"
#include "..\Utils\Timer.h"
#include "..\Game\Console.h"

#include "Engine/Core/Engine.h"          // ← Engine singleton
#include "Source/Utilities/CrashHandler.h"

// -----------------------------------------------------------------------------
// Globals & constants
// -----------------------------------------------------------------------------
constexpr int MAX_LOADSTRING = 100;

HINSTANCE                          g_hInst;
WCHAR                              g_szTitle[MAX_LOADSTRING];
WCHAR                              g_szClass[MAX_LOADSTRING];

// Classic systems
std::unique_ptr<GraphicsEngine>    g_graphics;
std::unique_ptr<Game>              g_game;
std::unique_ptr<InputManager>      g_input;
std::unique_ptr<Timer>             g_timer;
Console                            g_console;

// Win32 fwd-decl
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
    // ------------------------------------------------
    // 1. Crash handler (runs before anything else)
    // ------------------------------------------------
    CrashConfig crashCfg;
    crashCfg.dumpPrefix         = L"SparkCrash";
    crashCfg.uploadURL          = "https://crash.placeholder.com/upload";
    crashCfg.captureScreenshot  = true;
    crashCfg.captureSystemInfo  = true;
    crashCfg.captureAllThreads  = true;
    crashCfg.zipBeforeUpload    = true;
    InstallCrashHandler(crashCfg);

    // ------------------------------------------------
    // 2. Stock Win32 setup
    // ------------------------------------------------
    LoadStringW(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FPS,       g_szClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
        return -1;

    // ------------------------------------------------
    // 3. Normal message loop
    // ------------------------------------------------
    HACCEL accel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FPS));
    MSG     msg  = {};
    g_timer      = std::make_unique<Timer>();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // Feed console first
            if (msg.message == WM_CHAR && g_console.HandleChar(static_cast<wchar_t>(msg.wParam)))
                continue;
            if (msg.message == WM_KEYDOWN && g_console.HandleKeyDown(msg.wParam))
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

            if (g_input) g_input->Update();
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
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_FPS));
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MAKEINTRESOURCEW(IDC_FPS);
    wc.lpszClassName = g_szClass;
    wc.hIconSm       = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wc);
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    g_hInst = hInst;
    HWND hWnd = CreateWindowW(
        g_szClass, g_szTitle,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 1280, 720,
        nullptr, nullptr, hInst, nullptr);
    if (!hWnd) return FALSE;

    // 1. Spark Engine bootstrap (owns main loop/tick internally)
    EngineConfig cfg;
    cfg.enableEditor   = true;
    cfg.projectPath    = "Assets/";
    cfg.windowWidth    = 1280;
    cfg.windowHeight   = 720;
    cfg.windowTitle    = L"Spark Engine";
    cfg.externalHwnd   = hWnd;                 // <— pass the Win32 window
    Engine& engine     = Engine::Instance();
    if (!engine.Initialize(cfg))
    {
        MessageBoxW(hWnd, L"Engine init failed", L"Fatal", MB_ICONERROR);
        return FALSE;
    }

    // 2. Classic subsystems (migrate into Engine later)
    g_console.Initialize(1280, 720);

    g_graphics = std::make_unique<GraphicsEngine>();
    if (FAILED(g_graphics->Initialize(hWnd)))
    {
        MessageBoxW(hWnd, L"Graphics init failed", L"Fatal", MB_ICONERROR);
        return FALSE;
    }

    g_input = std::make_unique<InputManager>();
    g_input->Initialize(hWnd);

    g_game = std::make_unique<Game>();
    g_game->Initialize(g_graphics.get(), g_input.get());

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
            if (wParam == VK_OEM_3) { g_console.Toggle(); return 0; }
            if (g_input) g_input->HandleMessage(msg, wParam, lParam);
            break;

        case WM_KEYUP:
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN: case WM_LBUTTONUP:
        case WM_RBUTTONDOWN: case WM_RBUTTONUP:
            if (g_input) g_input->HandleMessage(msg, wParam, lParam);
            break;

        case WM_SIZE:
            if (g_graphics) g_graphics->OnResize(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_DESTROY:
            Engine::Instance().Shutdown();
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
        EndDialog(hDlg, LOWORD(wParam)); return TRUE;
    }
    return FALSE;
}