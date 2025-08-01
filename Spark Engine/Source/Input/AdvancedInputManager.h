#pragma once
#include "../Core/framework.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include <bitset>
#include <DirectXMath.h>

namespace SparkEngine {
    enum class InputDevice {
        Keyboard,
        Mouse,
        Gamepad
    };

    enum class InputEventType {
        KeyDown,
        KeyUp,
        MouseMove,
        MouseButtonDown,
        MouseButtonUp,
        MouseWheel,
        GamepadButtonDown,
        GamepadButtonUp,
        GamepadAxisMove
    };

    struct InputEvent {
        InputEventType type;
        union {
            struct { uint32_t keyCode; } key;
            struct { int32_t x, y; int32_t deltaX, deltaY; } mouse;
            struct { uint32_t button; } mouseButton;
            struct { float delta; } mouseWheel;
            struct { uint32_t button; uint32_t gamepadId; } gamepadButton;
            struct { uint32_t axis; float value; uint32_t gamepadId; } gamepadAxis;
        };
        uint64_t timestamp;
    };

    struct InputBinding {
        std::string name;
        std::vector<uint32_t> keys;
        std::vector<uint32_t> mouseButtons;
        std::vector<uint32_t> gamepadButtons;
        std::function<void(float)> callback;
        bool isAxis = false;
        float deadzone = 0.1f;
        float scale = 1.0f;
    };

    class AdvancedInputManager {
    public:
        AdvancedInputManager();
        ~AdvancedInputManager();

        bool Initialize(HWND hwnd);
        void Shutdown();
        void Update();

        // Raw input processing
        void HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
        bool ProcessRawInput(LPARAM lParam);

        // Key state queries
        bool IsKeyDown(uint32_t keyCode) const;
        bool IsKeyPressed(uint32_t keyCode) const;
        bool IsKeyReleased(uint32_t keyCode) const;

        // Mouse state queries
        bool IsMouseButtonDown(uint32_t button) const;
        bool IsMouseButtonPressed(uint32_t button) const;
        bool IsMouseButtonReleased(uint32_t button) const;
        DirectX::XMFLOAT2 GetMousePosition() const { return m_mousePosition; }
        DirectX::XMFLOAT2 GetMouseDelta() const { return m_mouseDelta; }
        float GetMouseWheelDelta() const { return m_mouseWheelDelta; }

        // Gamepad support
        bool IsGamepadConnected(uint32_t gamepadId) const;
        bool IsGamepadButtonDown(uint32_t gamepadId, uint32_t button) const;
        bool IsGamepadButtonPressed(uint32_t gamepadId, uint32_t button) const;
        float GetGamepadAxis(uint32_t gamepadId, uint32_t axis) const;

        // Input binding system
        void CreateInputBinding(const std::string& name, const InputBinding& binding);
        void RemoveInputBinding(const std::string& name);
        void ProcessInputBindings();

        // Event system
        void AddEventListener(std::function<void(const InputEvent&)> listener);
        void RemoveEventListener(size_t listenerId);

        // Input mapping and profiles
        void LoadInputProfile(const std::string& profileName);
        void SaveInputProfile(const std::string& profileName);
        void SetActiveProfile(const std::string& profileName);

        // Collaborative input features
        void EnableCollaborativeMode(bool enable) { m_collaborativeMode = enable; }
        void SetCollaborativeCursorCallback(std::function<void(const DirectX::XMFLOAT3&)> callback) {
            m_collaborativeCursorCallback = callback;
        }

        // Input recording and playback
        void StartRecording(const std::string& filename);
        void StopRecording();
        void StartPlayback(const std::string& filename);
        void StopPlayback();
        bool IsRecording() const { return m_recording; }
        bool IsPlayingBack() const { return m_playingBack; }

    private:
        struct KeyState {
            bool current = false;
            bool previous = false;
        };

        struct GamepadState {
            bool connected = false;
            std::bitset<16> buttons;
            std::bitset<16> previousButtons;
            float axes[8] = {0.0f};
            float previousAxes[8] = {0.0f};
        };

        void UpdateKeyStates();
        void UpdateMouseState();
        void UpdateGamepadStates();
        void ProcessInputEvent(const InputEvent& event);
        void RecordInputEvent(const InputEvent& event);

        HWND m_hwnd = nullptr;
        
        // Keyboard state
        std::unordered_map<uint32_t, KeyState> m_keyStates;
        
        // Mouse state
        std::unordered_map<uint32_t, KeyState> m_mouseButtonStates;
        DirectX::XMFLOAT2 m_mousePosition = {0.0f, 0.0f};
        DirectX::XMFLOAT2 m_mouseDelta = {0.0f, 0.0f};
        float m_mouseWheelDelta = 0.0f;
        
        // Gamepad state
        std::unordered_map<uint32_t, GamepadState> m_gamepadStates;
        
        // Input bindings
        std::unordered_map<std::string, InputBinding> m_inputBindings;
        
        // Event listeners
        std::vector<std::function<void(const InputEvent&)>> m_eventListeners;
        
        // Input profiles
        std::unordered_map<std::string, std::unordered_map<std::string, InputBinding>> m_inputProfiles;
        std::string m_activeProfile;
        
        // Collaborative features
        bool m_collaborativeMode = false;
        std::function<void(const DirectX::XMFLOAT3&)> m_collaborativeCursorCallback;
        
        // Recording and playback
        bool m_recording = false;
        bool m_playingBack = false;
        std::string m_recordingFilename;
        std::vector<InputEvent> m_recordedEvents;
        size_t m_playbackIndex = 0;
        uint64_t m_playbackStartTime = 0;
    };

    // Input system integration
    class InputSystem {
    public:
        InputSystem(AdvancedInputManager* inputManager) : m_inputManager(inputManager) {}

        bool Initialize(class EntityRegistry* registry);
        void Update(float deltaTime);

        AdvancedInputManager* GetInputManager() const { return m_inputManager; }

    private:
        AdvancedInputManager* m_inputManager;
        class EntityRegistry* m_registry = nullptr;
    };
}
