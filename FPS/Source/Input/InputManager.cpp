#include "InputManager.h"

InputManager::InputManager()
    : m_mouseX(0), m_mouseY(0)
    , m_prevMouseX(0), m_prevMouseY(0)
    , m_mouseDeltaX(0), m_mouseDeltaY(0)
    , m_hwnd(nullptr)
    , m_mouseCaptured(false)
{
    ZeroMemory(m_mouseButtons, sizeof(m_mouseButtons));
    ZeroMemory(m_prevMouseButtons, sizeof(m_prevMouseButtons));
}

InputManager::~InputManager()
{
    if (m_mouseCaptured)
    {
        CaptureMouse(false);
    }
}

void InputManager::Initialize(HWND hwnd)
{
    m_hwnd = hwnd;
    
    // Center mouse in window
    RECT rect;
    GetClientRect(hwnd, &rect);
    POINT center = { rect.right / 2, rect.bottom / 2 };
    ClientToScreen(hwnd, &center);
    SetCursorPos(center.x, center.y);
    
    m_mouseX = m_prevMouseX = center.x;
    m_mouseY = m_prevMouseY = center.y;
}

void InputManager::Update()
{
    // Store previous states
    m_prevKeyStates = m_keyStates;
    memcpy(m_prevMouseButtons, m_mouseButtons, sizeof(m_mouseButtons));
    
    m_prevMouseX = m_mouseX;
    m_prevMouseY = m_mouseY;
    
    // Update mouse delta
    if (m_mouseCaptured)
    {
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(m_hwnd, &cursor);
        
        m_mouseDeltaX = cursor.x - m_prevMouseX;
        m_mouseDeltaY = cursor.y - m_prevMouseY;
        
        // Re-center mouse for continuous movement
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        POINT center = { rect.right / 2, rect.bottom / 2 };
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
}

void InputManager::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        UpdateKeyState((int)wParam, true);
        break;
    case WM_KEYUP:
        UpdateKeyState((int)wParam, false);
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
        {
            UpdateMousePosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        break;
    }
}

bool InputManager::IsKeyDown(int key) const
{
    auto it = m_keyStates.find(key);
    return it != m_keyStates.end() ? it->second : false;
}

bool InputManager::IsKeyUp(int key) const
{
    return !IsKeyDown(key);
}

bool InputManager::WasKeyPressed(int key) const
{
    bool currentState = IsKeyDown(key);
    auto it = m_prevKeyStates.find(key);
    bool prevState = it != m_prevKeyStates.end() ? it->second : false;
    return currentState && !prevState;
}

bool InputManager::WasKeyReleased(int key) const
{
    bool currentState = IsKeyDown(key);
    auto it = m_prevKeyStates.find(key);
    bool prevState = it != m_prevKeyStates.end() ? it->second : false;
    return !currentState && prevState;
}

bool InputManager::IsMouseButtonDown(int button) const
{
    return button >= 0 && button < 3 ? m_mouseButtons[button] : false;
}

bool InputManager::WasMouseButtonPressed(int button) const
{
    if (button < 0 || button >= 3) return false;
    return m_mouseButtons[button] && !m_prevMouseButtons[button];
}

bool InputManager::WasMouseButtonReleased(int button) const
{
    if (button < 0 || button >= 3) return false;
    return !m_mouseButtons[button] && m_prevMouseButtons[button];
}

bool InputManager::GetMouseDelta(int& deltaX, int& deltaY) const
{
    deltaX = m_mouseDeltaX;
    deltaY = m_mouseDeltaY;
    return m_mouseDeltaX != 0 || m_mouseDeltaY != 0;
}

void InputManager::GetMousePosition(int& x, int& y) const
{
    x = m_mouseX;
    y = m_mouseY;
}

void InputManager::CaptureMouse(bool capture)
{
    if (capture && !m_mouseCaptured)
    {
        SetCapture(m_hwnd);
        ShowCursor(FALSE);
        m_mouseCaptured = true;
    }
    else if (!capture && m_mouseCaptured)
    {
        ReleaseCapture();
        ShowCursor(TRUE);
        m_mouseCaptured = false;
    }
}

void InputManager::UpdateKeyState(int key, bool isDown)
{
    m_keyStates[key] = isDown;
    
    // Handle escape key to release mouse
    if (key == VK_ESCAPE && !isDown && m_mouseCaptured)
    {
        CaptureMouse(false);
    }
}

void InputManager::UpdateMouseButton(int button, bool isDown)
{
    if (button >= 0 && button < 3)
    {
        m_mouseButtons[button] = isDown;
    }
}

void InputManager::UpdateMousePosition(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}
