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

class SparkEngineCamera
{
private:
    XMFLOAT3 m_position{ 0,0,0 };
    XMFLOAT3 m_forward{ 0,0,1 };
    XMFLOAT3 m_right{ 1,0,0 };
    XMFLOAT3 m_up{ 0,1,0 };
    float    m_pitch{ 0 }, m_yaw{ 0 }, m_roll{ 0 };

    XMMATRIX m_viewMatrix{};
    XMMATRIX m_projectionMatrix{};

    float m_moveSpeed{ 10.0f };
    float m_rotationSpeed{ 2.0f };
    float m_defaultFov{ DirectX::XM_PIDIV2 };
    float m_zoomedFov{ DirectX::XM_PIDIV2 / 2.0f };

public:
    SparkEngineCamera() = default;
    ~SparkEngineCamera() = default;

    // Must call before movement
    void Initialize(float aspectRatio);

    // Update per-frame (recomputes view matrix)
    void Update(float deltaTime);

    // Movement
    void MoveForward(float amount);
    void MoveRight(float amount);
    void MoveUp(float amount);

    // Rotation
    void Pitch(float angle);
    void Yaw(float angle);
	void Roll(float angle);

    // Zoom toggle
    void SetZoom(bool enabled);

    // Direct set
    void SetPosition(const XMFLOAT3& pos)
    {
        m_position = pos;
        UpdateViewMatrix();
    }

    // Accessors
    const XMMATRIX& GetViewMatrix()       const { return m_viewMatrix; }
    const XMMATRIX& GetProjectionMatrix() const { return m_projectionMatrix; }
    const XMFLOAT3& GetPosition()         const { return m_position; }
    const XMFLOAT3& GetForward()          const { return m_forward; }

private:
    void UpdateViewMatrix();
};
