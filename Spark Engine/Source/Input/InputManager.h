#pragma once
#include "..\Core\framework.h"
#include <unordered_map>

class InputManager
{
private:
    std::unordered_map<int, bool> m_keyStates;
    std::unordered_map<int, bool> m_prevKeyStates;
    
    bool m_mouseButtons[3]; // Left, Right, Middle
    bool m_prevMouseButtons[3];
    
    int m_mouseX, m_mouseY;
    int m_prevMouseX, m_prevMouseY;
    int m_mouseDeltaX, m_mouseDeltaY;
    
    HWND m_hwnd;
    bool m_mouseCaptured;

public:
    InputManager();
    ~InputManager();

    void Initialize(HWND hwnd);
    void Update();
    void HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

    // Keyboard
    bool IsKeyDown(int key) const;
    bool IsKeyUp(int key) const;
    bool WasKeyPressed(int key) const;
    bool WasKeyReleased(int key) const;

    // Mouse
    bool IsMouseButtonDown(int button) const;
    bool WasMouseButtonPressed(int button) const;
    bool WasMouseButtonReleased(int button) const;
    bool GetMouseDelta(int& deltaX, int& deltaY) const;
    void GetMousePosition(int& x, int& y) const;

    // Mouse capture for SparkEngine controls
    void CaptureMouse(bool capture);
    bool IsMouseCaptured() const { return m_mouseCaptured; }

private:
    void UpdateKeyState(int key, bool isDown);
    void UpdateMouseButton(int button, bool isDown);
    void UpdateMousePosition(int x, int y);
};

