/**
 * @file InputManager.h
 * @brief Comprehensive input handling system for keyboard and mouse
 * @author Spark Engine Team
 * @date 2024
 * 
 * The InputManager class provides a complete input handling system that processes
 * keyboard and mouse input events, tracks button states, calculates mouse movement
 * deltas, and provides convenient query methods for input state. It supports
 * mouse capture for first-person camera controls and maintains both current and
 * previous frame states for edge detection.
 */

#pragma once

#include "Utils/Assert.h"
#include "..\Core\framework.h"
#include <unordered_map>

/**
 * @brief Input management system for keyboard and mouse
 * 
 * The InputManager class handles all user input processing for the Spark Engine.
 * It processes Windows messages for keyboard and mouse events, maintains state
 * tracking for input queries, and provides mouse capture functionality for
 * first-person controls.
 * 
 * Features include:
 * - Keyboard state tracking with press/release detection
 * - Mouse button state tracking with press/release detection
 * - Mouse position and delta movement tracking
 * - Mouse capture/release for camera controls
 * - Frame-based state management for edge detection
 * - Windows message handling integration
 * 
 * @note Input states are updated once per frame via Update()
 * @warning Initialize() must be called with a valid window handle before use
 */
class InputManager
{
private:
    std::unordered_map<int, bool> m_keyStates;     ///< Current frame keyboard states
    std::unordered_map<int, bool> m_prevKeyStates; ///< Previous frame keyboard states

    bool m_mouseButtons[3];      ///< Current frame mouse button states [Left, Right, Middle]
    bool m_prevMouseButtons[3];  ///< Previous frame mouse button states

    int  m_mouseX, m_mouseY;           ///< Current mouse position
    int  m_prevMouseX, m_prevMouseY;   ///< Previous frame mouse position
    int  m_mouseDeltaX, m_mouseDeltaY; ///< Mouse movement delta since last frame

    HWND m_hwnd;           ///< Window handle for mouse capture
    bool m_mouseCaptured;  ///< Whether mouse is currently captured

public:
    /**
     * @brief Default constructor
     * 
     * Initializes input manager with default values. Call Initialize()
     * before processing any input.
     */
    InputManager();

    /**
     * @brief Destructor
     * 
     * Automatically releases mouse capture if it was active.
     */
    ~InputManager();

    /**
     * @brief Initialize the input manager with window handle
     * 
     * Sets up the input manager with the specified window handle for
     * mouse capture functionality and input processing.
     * 
     * @param hwnd Handle to the window that will receive input
     */
    void Initialize(HWND hwnd);

    /**
     * @brief Update input states for the current frame
     * 
     * Processes accumulated input changes and updates state tracking.
     * Should be called once per frame before querying input states.
     * Stores current states as previous states for next frame.
     */
    void Update();

    /**
     * @brief Process Windows input messages
     * 
     * Handles WM_KEYDOWN, WM_KEYUP, WM_LBUTTONDOWN, WM_LBUTTONUP,
     * WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP,
     * and WM_MOUSEMOVE messages to update input states.
     * 
     * @param message Windows message type
     * @param wParam Message-specific parameter (key code, etc.)
     * @param lParam Message-specific parameter (mouse coordinates, etc.)
     */
    void HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Check if a key is currently pressed
     * 
     * Returns true if the specified key is currently being held down.
     * 
     * @param key Virtual key code (e.g., VK_SPACE, 'W', VK_ESCAPE)
     * @return true if key is currently pressed, false otherwise
     */
    bool IsKeyDown(int key) const;

    /**
     * @brief Check if a key is currently released
     * 
     * Returns true if the specified key is currently not being pressed.
     * 
     * @param key Virtual key code (e.g., VK_SPACE, 'W', VK_ESCAPE)
     * @return true if key is currently not pressed, false otherwise
     */
    bool IsKeyUp(int key) const;

    /**
     * @brief Check if a key was just pressed this frame
     * 
     * Returns true if the key was released last frame and pressed this frame.
     * Useful for detecting single key press events.
     * 
     * @param key Virtual key code (e.g., VK_SPACE, 'W', VK_ESCAPE)
     * @return true if key was just pressed, false otherwise
     */
    bool WasKeyPressed(int key) const;

    /**
     * @brief Check if a key was just released this frame
     * 
     * Returns true if the key was pressed last frame and released this frame.
     * Useful for detecting single key release events.
     * 
     * @param key Virtual key code (e.g., VK_SPACE, 'W', VK_ESCAPE)
     * @return true if key was just released, false otherwise
     */
    bool WasKeyReleased(int key) const;

    /**
     * @brief Check if a mouse button is currently pressed
     * 
     * Returns true if the specified mouse button is currently being held down.
     * 
     * @param button Mouse button index (0=Left, 1=Right, 2=Middle)
     * @return true if mouse button is currently pressed, false otherwise
     */
    bool IsMouseButtonDown(int button) const;

    /**
     * @brief Check if a mouse button was just pressed this frame
     * 
     * Returns true if the button was released last frame and pressed this frame.
     * 
     * @param button Mouse button index (0=Left, 1=Right, 2=Middle)
     * @return true if mouse button was just pressed, false otherwise
     */
    bool WasMouseButtonPressed(int button) const;

    /**
     * @brief Check if a mouse button was just released this frame
     * 
     * Returns true if the button was pressed last frame and released this frame.
     * 
     * @param button Mouse button index (0=Left, 1=Right, 2=Middle)
     * @return true if mouse button was just released, false otherwise
     */
    bool WasMouseButtonReleased(int button) const;

    /**
     * @brief Get mouse movement delta since last frame
     * 
     * Returns the pixel delta movement of the mouse cursor since the last frame.
     * Useful for camera controls and mouse look functionality.
     * 
     * @param deltaX Output parameter for horizontal mouse movement
     * @param deltaY Output parameter for vertical mouse movement
     * @return true if delta values are valid, false otherwise
     */
    bool GetMouseDelta(int& deltaX, int& deltaY) const;

    /**
     * @brief Get current mouse cursor position
     * 
     * Returns the current pixel coordinates of the mouse cursor relative
     * to the client area of the window.
     * 
     * @param x Output parameter for horizontal cursor position
     * @param y Output parameter for vertical cursor position
     */
    void GetMousePosition(int& x, int& y) const;

    /**
     * @brief Capture or release the mouse cursor
     * 
     * When captured, the mouse cursor is hidden and confined to the window,
     * and movement generates delta values for camera controls. When released,
     * the cursor becomes visible and can move freely.
     * 
     * @param capture true to capture mouse, false to release
     */
    void CaptureMouse(bool capture);

    /**
     * @brief Check if the mouse is currently captured
     * @return true if mouse is captured, false if free
     */
    bool IsMouseCaptured() const { return m_mouseCaptured; }

private:
    /**
     * @brief Update the state of a specific key
     * 
     * Internal method for updating keyboard state from Windows messages.
     * 
     * @param key Virtual key code to update
     * @param isDown true if key is pressed, false if released
     */
    void UpdateKeyState(int key, bool isDown);

    /**
     * @brief Update the state of a specific mouse button
     * 
     * Internal method for updating mouse button state from Windows messages.
     * 
     * @param button Mouse button index (0=Left, 1=Right, 2=Middle)
     * @param isDown true if button is pressed, false if released
     */
    void UpdateMouseButton(int button, bool isDown);

    /**
     * @brief Update mouse position and calculate delta
     * 
     * Internal method for updating mouse position from Windows messages
     * and calculating movement delta.
     * 
     * @param x New horizontal mouse position
     * @param y New vertical mouse position
     */
    void UpdateMousePosition(int x, int y);
};