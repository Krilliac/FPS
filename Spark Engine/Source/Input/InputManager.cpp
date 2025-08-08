// InputManager.cpp
#include "InputManager.h"
#include "Utils/Assert.h"
#include <Windows.h>
#include <cstring>
#include <iostream>

InputManager::InputManager()
    : m_mouseX(0), m_mouseY(0)
    , m_prevMouseX(0), m_prevMouseY(0)
    , m_mouseDeltaX(0), m_mouseDeltaY(0)
    , m_hwnd(nullptr)
    , m_mouseCaptured(false)
{
    ZeroMemory(m_mouseButtons, sizeof(m_mouseButtons));
    ZeroMemory(m_prevMouseButtons, sizeof(m_prevMouseButtons));
    std::wcout << L"[INFO] InputManager constructed." << std::endl;
}

InputManager::~InputManager()
{
    std::wcout << L"[INFO] InputManager destructor called." << std::endl;
    if (m_mouseCaptured)
        CaptureMouse(false);
}

void InputManager::Initialize(HWND hwnd)
{
    ASSERT_MSG(hwnd != nullptr, "InputManager::Initialize - hwnd is null");
    m_hwnd = hwnd;

    RECT rect;
    GetClientRect(hwnd, &rect);
    POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };
    ClientToScreen(hwnd, &center);
    SetCursorPos(center.x, center.y);

    m_mouseX = m_prevMouseX = center.x;
    m_mouseY = m_prevMouseY = center.y;
}

void InputManager::Update()
{
    std::wcout << L"[OPERATION] InputManager::Update called." << std::endl;
    ASSERT_MSG(m_hwnd != nullptr, "InputManager::Update - hwnd not initialized");

    m_prevKeyStates = m_keyStates;
    memcpy(m_prevMouseButtons, m_mouseButtons, sizeof(m_mouseButtons));

    m_prevMouseX = m_mouseX;
    m_prevMouseY = m_mouseY;

    if (m_mouseCaptured)
    {
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(m_hwnd, &cursor);

        m_mouseDeltaX = cursor.x - m_prevMouseX;
        m_mouseDeltaY = cursor.y - m_prevMouseY;

        RECT rect;
        GetClientRect(m_hwnd, &rect);
        POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };
        ClientToScreen(m_hwnd, &center);
        SetCursorPos(center.x, center.y);

        m_mouseX = center.x;
        m_mouseY = center.y;
    }
    else
    {
        m_mouseDeltaX = m_mouseX - m_prevMouseX;
        m_mouseDeltaY = m_mouseY - m_prevMouseY;
    }
    std::wcout << L"[INFO] InputManager update complete." << std::endl;
}

void InputManager::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        UpdateKeyState(static_cast<int>(wParam), true);
        break;
    case WM_KEYUP:
        UpdateKeyState(static_cast<int>(wParam), false);
        break;
    case WM_LBUTTONDOWN:
        UpdateMouseButton(0, true);
        if (!m_mouseCaptured) CaptureMouse(true);
        break;
    case WM_LBUTTONUP:
        UpdateMouseButton(0, false);
        break;
    case WM_RBUTTONDOWN:
        UpdateMouseButton(1, true);
        break;
    case WM_RBUTTONUP:
        UpdateMouseButton(1, false);
        break;
    case WM_MBUTTONDOWN:
        UpdateMouseButton(2, true);
        break;
    case WM_MBUTTONUP:
        UpdateMouseButton(2, false);
        break;
    case WM_MOUSEMOVE:
        if (!m_mouseCaptured)
            UpdateMousePosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    default:
        break;
    }
}

bool InputManager::IsKeyDown(int key) const
{
    std::wcout << L"[OPERATION] InputManager::IsKeyDown called. key=" << key << std::endl;
    ASSERT_MSG(key >= 0, "IsKeyDown - invalid key code");
    auto it = m_keyStates.find(key);
    bool result = it != m_keyStates.end() && it->second;
    std::wcout << L"[INFO] InputManager::IsKeyDown result=" << result << std::endl;
    return result;
}

bool InputManager::IsKeyUp(int key) const
{
    return !IsKeyDown(key);
}

bool InputManager::WasKeyPressed(int key) const
{
    std::wcout << L"[OPERATION] InputManager::WasKeyPressed called. key=" << key << std::endl;
    ASSERT_MSG(key >= 0, "WasKeyPressed - invalid key code");
    bool curr = IsKeyDown(key);
    bool prev = m_prevKeyStates.count(key) ? m_prevKeyStates.at(key) : false;
    bool result = curr && !prev;
    std::wcout << L"[INFO] InputManager::WasKeyPressed result=" << result << std::endl;
    return result;
}

bool InputManager::WasKeyReleased(int key) const
{
    ASSERT_MSG(key >= 0, "WasKeyReleased - invalid key code");
    bool curr = IsKeyDown(key);
    bool prev = m_prevKeyStates.count(key) ? m_prevKeyStates.at(key) : false;
    return !curr && prev;
}

bool InputManager::IsMouseButtonDown(int button) const
{
    std::wcout << L"[OPERATION] InputManager::IsMouseButtonDown called. button=" << button << std::endl;
    ASSERT_MSG(button >= 0 && button < 3, "IsMouseButtonDown - invalid button");
    bool result = m_mouseButtons[button];
    std::wcout << L"[INFO] InputManager::IsMouseButtonDown result=" << result << std::endl;
    return result;
}

bool InputManager::WasMouseButtonPressed(int button) const
{
    std::wcout << L"[OPERATION] InputManager::WasMouseButtonPressed called. button=" << button << std::endl;
    ASSERT_MSG(button >= 0 && button < 3, "WasMouseButtonPressed - invalid button");
    bool result = m_mouseButtons[button] && !m_prevMouseButtons[button];
    std::wcout << L"[INFO] InputManager::WasMouseButtonPressed result=" << result << std::endl;
    return result;
}

bool InputManager::WasMouseButtonReleased(int button) const
{
    ASSERT_MSG(button >= 0 && button < 3, "WasMouseButtonReleased - invalid button");
    return !m_mouseButtons[button] && m_prevMouseButtons[button];
}

bool InputManager::GetMouseDelta(int& deltaX, int& deltaY) const
{
    std::wcout << L"[OPERATION] InputManager::GetMouseDelta called." << std::endl;
    deltaX = m_mouseDeltaX;
    deltaY = m_mouseDeltaY;
    bool result = (m_mouseDeltaX != 0 || m_mouseDeltaY != 0);
    std::wcout << L"[INFO] InputManager::GetMouseDelta result=" << result << std::endl;
    return result;
}

void InputManager::GetMousePosition(int& x, int& y) const
{
    x = m_mouseX;
    y = m_mouseY;
}

void InputManager::CaptureMouse(bool capture)
{
    if (capture)
    {
        ASSERT_MSG(!m_mouseCaptured, "CaptureMouse: already captured");
        SetCapture(m_hwnd);
        ShowCursor(FALSE);
        m_mouseCaptured = true;
        std::wcout << L"[INFO] Mouse captured." << std::endl;
    }
    else
    {
        ASSERT_MSG(m_mouseCaptured, "CaptureMouse: not captured");
        ReleaseCapture();
        ShowCursor(TRUE);
        m_mouseCaptured = false;
        std::wcout << L"[INFO] Mouse capture released." << std::endl;
    }
}

void InputManager::UpdateKeyState(int key, bool isDown)
{
    ASSERT_MSG(key >= 0, "UpdateKeyState - invalid key code");
    m_keyStates[key] = isDown;
    if (key == VK_ESCAPE && !isDown && m_mouseCaptured)
        CaptureMouse(false);
}

void InputManager::UpdateMouseButton(int button, bool isDown)
{
    ASSERT_MSG(button >= 0 && button < 3, "UpdateMouseButton - invalid button");
    m_mouseButtons[button] = isDown;
}

void InputManager::UpdateMousePosition(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}