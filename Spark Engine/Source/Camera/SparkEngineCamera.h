/**
 * @file SparkEngineCamera.h
 * @brief First-person camera system with smooth movement and controls
 * @author Spark Engine Team
 * @date 2025
 * 
 * This class provides a comprehensive first-person camera implementation with
 * smooth movement, mouse look controls, zoom functionality, and proper matrix
 * calculations for 3D rendering. The camera supports WASD movement, mouse look,
 * vertical movement, and dynamic field of view adjustment.
 */

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <windows.h>
#include <DirectXMath.h>
#include <algorithm>           // std::clamp
#include "Utils/Assert.h"      // custom assert

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

/**
 * @brief First-person camera controller for 3D navigation
 * 
 * The SparkEngineCamera class provides a complete first-person camera system
 * with smooth movement, mouse look controls, and configurable parameters.
 * It handles view and projection matrix calculations, movement input processing,
 * and provides zoom functionality for gameplay mechanics.
 * 
 * Features include:
 * - Smooth first-person movement (forward, right, up)
 * - Mouse look with pitch, yaw, and roll controls
 * - Configurable movement and rotation speeds
 * - Zoom functionality with different FOV settings
 * - Automatic view matrix updates
 * - Pitch clamping to prevent over-rotation
 * 
 * @note Camera uses right-handed coordinate system with Y-up
 * @warning Initialize() must be called before any movement or matrix operations
 */
class SparkEngineCamera
{
private:
    XMFLOAT3 m_position{ 0,0,0 };  ///< Camera position in world space
    XMFLOAT3 m_forward{ 0,0,1 };   ///< Camera forward direction vector
    XMFLOAT3 m_right{ 1,0,0 };     ///< Camera right direction vector
    XMFLOAT3 m_up{ 0,1,0 };        ///< Camera up direction vector
    float    m_pitch{ 0 }, m_yaw{ 0 }, m_roll{ 0 }; ///< Camera rotation angles in radians

    XMMATRIX m_viewMatrix{};       ///< Cached view transformation matrix
    XMMATRIX m_projectionMatrix{}; ///< Cached projection transformation matrix

    float m_moveSpeed{ 10.0f };        ///< Movement speed in units per second
    float m_rotationSpeed{ 2.0f };     ///< Rotation speed multiplier
    float m_defaultFov{ DirectX::XM_PIDIV2 };      ///< Default field of view (90 degrees)
    float m_zoomedFov{ DirectX::XM_PIDIV2 / 2.0f }; ///< Zoomed field of view (45 degrees)

public:
    /**
     * @brief Default constructor
     * 
     * Initializes camera with default values. Call Initialize() before use.
     */
    SparkEngineCamera() = default;

    /**
     * @brief Default destructor
     * 
     * Uses default cleanup as all resources are managed automatically.
     */
    ~SparkEngineCamera() = default;

    /**
     * @brief Initialize the camera with projection settings
     * 
     * Sets up the camera with the specified aspect ratio and calculates
     * the initial projection matrix. Must be called before movement operations.
     * 
     * @param aspectRatio Screen width divided by height (e.g., 16/9 = 1.777)
     * @note Call this after graphics system initialization and window creation
     */
    void Initialize(float aspectRatio);

    /**
     * @brief Update camera for the current frame
     * 
     * Recalculates the view matrix based on current position and orientation.
     * Should be called once per frame after processing movement input.
     * 
     * @param deltaTime Time elapsed since last frame in seconds (currently unused)
     */
    void Update(float deltaTime);

    /**
     * @brief Move camera forward or backward
     * 
     * Moves the camera along its forward direction vector. Positive values
     * move forward, negative values move backward.
     * 
     * @param amount Distance to move (positive = forward, negative = backward)
     */
    void MoveForward(float amount);

    /**
     * @brief Move camera left or right
     * 
     * Moves the camera along its right direction vector. Positive values
     * move right, negative values move left.
     * 
     * @param amount Distance to move (positive = right, negative = left)
     */
    void MoveRight(float amount);

    /**
     * @brief Move camera up or down
     * 
     * Moves the camera along its up direction vector. Positive values
     * move up, negative values move down.
     * 
     * @param amount Distance to move (positive = up, negative = down)
     */
    void MoveUp(float amount);

    /**
     * @brief Rotate camera around X-axis (look up/down)
     * 
     * Adjusts the camera's pitch rotation. Positive values pitch up,
     * negative values pitch down. Pitch is automatically clamped to
     * prevent over-rotation.
     * 
     * @param angle Pitch angle to add in radians
     */
    void Pitch(float angle);

    /**
     * @brief Rotate camera around Y-axis (look left/right)
     * 
     * Adjusts the camera's yaw rotation. Positive values yaw right,
     * negative values yaw left.
     * 
     * @param angle Yaw angle to add in radians
     */
    void Yaw(float angle);

    /**
     * @brief Rotate camera around Z-axis (tilt left/right)
     * 
     * Adjusts the camera's roll rotation. Positive values roll clockwise,
     * negative values roll counter-clockwise.
     * 
     * @param angle Roll angle to add in radians
     */
	void Roll(float angle);

    /**
     * @brief Toggle between normal and zoomed field of view
     * 
     * Switches between default and zoomed FOV settings, useful for
     * aiming mechanics or telescope effects.
     * 
     * @param enabled true to enable zoom (narrow FOV), false for normal FOV
     */
    void SetZoom(bool enabled);

    /**
     * @brief Directly set the camera position
     * 
     * Immediately positions the camera at the specified world coordinates
     * and updates the view matrix.
     * 
     * @param pos New position in world coordinates
     */
    void SetPosition(const XMFLOAT3& pos)
    {
        m_position = pos;
        UpdateViewMatrix();
    }

    /**
     * @brief Get the current view transformation matrix
     * @return 4x4 view matrix for rendering transformations
     */
    const XMMATRIX& GetViewMatrix()       const { return m_viewMatrix; }

    /**
     * @brief Get the current projection transformation matrix
     * @return 4x4 projection matrix for rendering transformations
     */
    const XMMATRIX& GetProjectionMatrix() const { return m_projectionMatrix; }

    /**
     * @brief Get the current camera position
     * @return Position vector in world coordinates
     */
    const XMFLOAT3& GetPosition()         const { return m_position; }

    /**
     * @brief Get the current camera forward direction
     * @return Normalized forward direction vector
     */
    const XMFLOAT3& GetForward()          const { return m_forward; }

private:
    /**
     * @brief Recalculate the view matrix from current transform
     * 
     * Updates the cached view matrix based on current position and rotation.
     * Called automatically when transform properties change.
     */
    void UpdateViewMatrix();
};
