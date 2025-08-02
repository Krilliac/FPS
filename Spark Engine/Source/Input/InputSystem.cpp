#include "InputSystem.h"
#include "../Utils/Logger.h"

namespace SparkEngine {
    InputSystem::InputSystem() {}
    InputSystem::~InputSystem() {}

    void InputSystem::Initialize(HWND hwnd) {
        Logger::Info("Initializing InputSystem...");
        m_hwnd = hwnd;
    }

    void InputSystem::Shutdown() {
        Logger::Info("Shutting down InputSystem...");
    }

    void InputSystem::Update() {
        // Keyboard
        for (int i = 0; i < 256; ++i) {
            m_keyStates[i] = GetAsyncKeyState(i);
        }
        // Mouse
        m_mouseButtons[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        m_mouseButtons[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        m_mouseButtons[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
        GetCursorPos(&m_mousePos);
        ScreenToClient(m_hwnd, &m_mousePos);
    }

    bool InputSystem::IsKeyPressed(int vKey) const {
        return (m_keyStates[vKey] & 0x8000) != 0;
    }

    bool InputSystem::IsMouseButtonPressed(int button) const {
        if (button < 0 || button > 2) return false;
        return m_mouseButtons[button];
    }

    std::pair<int,int> InputSystem::GetMousePosition() const {
        return { m_mousePos.x, m_mousePos.y };
    }
}
