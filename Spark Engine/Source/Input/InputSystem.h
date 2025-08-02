#pragma once
#include "../Core/framework.h"

namespace SparkEngine {
    class InputSystem {
    public:
        InputSystem();
        ~InputSystem();

        void Initialize(HWND hwnd);
        void Shutdown();
        void Update();

        bool IsKeyPressed(int vKey) const;
        bool IsMouseButtonPressed(int button) const;
        std::pair<int,int> GetMousePosition() const;

    private:
        HWND          m_hwnd = nullptr;
        SHORT         m_keyStates[256]{};
        bool          m_mouseButtons[3]{};
        POINT         m_mousePos{0,0};
    };
}
