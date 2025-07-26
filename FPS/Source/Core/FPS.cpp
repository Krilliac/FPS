// FPS.cpp : Defines the entry point for the application.
#include "framework.h"
#include "FPS.h"

#include <Windows.h>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <DirectXMath.h>

#include "..\Graphics\GraphicsEngine.h"
#include "..\Game\Game.h"
#include "..\Input\InputManager.h"
#include "..\Utils\Timer.h"
#include "..\Game\Console.h"

constexpr auto MAX_LOADSTRING = 100;

// Global Variables
HINSTANCE                             hInst;
WCHAR                                 szTitle[MAX_LOADSTRING];
WCHAR                                 szWindowClass[MAX_LOADSTRING];

// Systems
std::unique_ptr<GraphicsEngine>       g_graphics;
std::unique_ptr<Game>                 g_game;
std::unique_ptr<InputManager>         g_input;
std::unique_ptr<Timer>                g_timer;
Console                                g_console;

// Forward declarations
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FPS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FPS));
    MSG    msg = {};
    g_timer = std::make_unique<Timer>();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // Console input handling
            if (msg.message == WM_CHAR)
            {
                if (g_console.HandleChar((wchar_t)msg.wParam))
                    continue;
            }
            else if (msg.message == WM_KEYDOWN)
            {
                if (g_console.HandleKeyDown(msg.wParam))
                    continue;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Update
            float dt = g_timer->GetDeltaTime();
            if (g_input) g_input->Update();
            if (g_game && !g_console.IsVisible())
                g_game->Update(dt);

            // Render sequence
            g_graphics->BeginFrame();         // clear & bind

            if (g_game)
                g_game->Render();            // draw terrain, objects, projectiles

            if (g_console.IsVisible())
                g_console.Render(g_graphics->GetContext());  // overlay

            g_graphics->EndFrame();           // present
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FPS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FPS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    HWND hWnd = CreateWindowW(
        szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1280, 720,
        nullptr, nullptr, hInstance, nullptr
    );
    if (!hWnd)
        return FALSE;

    // Initialize console overlay
    g_console.Initialize(1280, 720);

    // Initialize graphics engine
    g_graphics = std::make_unique<GraphicsEngine>();
    if (FAILED(g_graphics->Initialize(hWnd)))
    {
        MessageBox(hWnd, L"Failed to initialize graphics engine", L"Error", MB_OK);
        return FALSE;
    }

    // Initialize input manager
    g_input = std::make_unique<InputManager>();
    g_input->Initialize(hWnd);

    // Initialize game logic
    g_game = std::make_unique<Game>();
    g_game->Initialize(g_graphics.get(), g_input.get());

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_ACTIVATE:
    {
        BOOL active = (LOWORD(wParam) != WA_INACTIVE);
        RECT rc; GetClientRect(hWnd, &rc);
        MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rc), 2);
        ClipCursor(active ? &rc : nullptr);
        return 0;
    }

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            ClipCursor(nullptr);
            ShowCursor(TRUE);
            return 0;
        }
        if (wParam == VK_OEM_3)  // back-tick `
        {
            g_console.Toggle();
            return 0;
        }
        // fallthrough to input manager
    case WM_KEYUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        if (g_input)
            g_input->HandleMessage(msg, wParam, lParam);
        break;

    case WM_SIZE:
        if (g_graphics)
            g_graphics->OnResize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_DESTROY:
        ClipCursor(nullptr);
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