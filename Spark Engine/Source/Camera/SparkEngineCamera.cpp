#include "SparkEngineCamera.h"
#include "Utils/Assert.h"
#include <DirectXMath.h>
#include <cmath>
#include <iostream>

using namespace DirectX;

void SparkEngineCamera::Initialize(float aspectRatio)
{
    ASSERT_MSG(aspectRatio > 0.0f, "Aspect ratio must be positive");
    m_projectionMatrix = XMMatrixPerspectiveFovLH(
        m_defaultFov, aspectRatio, 0.1f, 1000.0f);
    UpdateViewMatrix();
}

void SparkEngineCamera::UpdateViewMatrix()
{
    // Build rotation matrix
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);

    // Transform basis vectors
    XMVECTOR fb = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rot);
    XMVECTOR rb = XMVector3TransformCoord(XMVectorSet(1, 0, 0, 0), rot);
    XMVECTOR ub = XMVector3TransformCoord(XMVectorSet(0, 1, 0, 0), rot);

    // Store updated basis
    XMStoreFloat3(&m_forward, fb);
    XMStoreFloat3(&m_right, rb);
    XMStoreFloat3(&m_up, ub);

    // Compute view matrix
    XMVECTOR pos = XMLoadFloat3(&m_position);
    m_viewMatrix = XMMatrixLookAtLH(pos, pos + fb, ub);

    std::wcerr << L"[Camera] UpdateViewMatrix pos=(" << m_position.x << L"," << m_position.y << L"," << m_position.z << L") pitch=" << m_pitch << L" yaw=" << m_yaw << L" roll=" << m_roll << std::endl;
}

void SparkEngineCamera::Update(float deltaTime)
{
    ASSERT_MSG(deltaTime >= 0.0f && std::isfinite(deltaTime),
        "Camera Update deltaTime must be non-negative and finite");
    UpdateViewMatrix();
    std::wcerr << L"[Camera] Update called, deltaTime=" << deltaTime << std::endl;
}

void SparkEngineCamera::MoveForward(float amount)
{
    ASSERT_MSG(std::isfinite(amount), "Move amount must be finite");
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR f = XMLoadFloat3(&m_forward);
    p = XMVectorAdd(p, XMVectorScale(f, amount * m_moveSpeed));
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::MoveRight(float amount)
{
    ASSERT_MSG(std::isfinite(amount), "Move amount must be finite");
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR r = XMLoadFloat3(&m_right);
    p = XMVectorAdd(p, XMVectorScale(r, amount * m_moveSpeed));
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::MoveUp(float amount)
{
    ASSERT_MSG(std::isfinite(amount), "Move amount must be finite");
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR u = XMLoadFloat3(&m_up);
    p = XMVectorAdd(p, XMVectorScale(u, amount * m_moveSpeed));
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::Pitch(float angle)
{
    ASSERT_MSG(std::isfinite(angle), "Angle must be finite");
    m_pitch = std::clamp(
        m_pitch + angle * m_rotationSpeed,
        -XM_PIDIV2 + 0.01f,
        XM_PIDIV2 - 0.01f);
    UpdateViewMatrix();
}

void SparkEngineCamera::Yaw(float angle)
{
    ASSERT_MSG(std::isfinite(angle), "Angle must be finite");
    m_yaw += angle * m_rotationSpeed;
    if (m_yaw > XM_2PI) m_yaw -= XM_2PI;
    if (m_yaw < 0.0f)   m_yaw += XM_2PI;
    UpdateViewMatrix();
}

void SparkEngineCamera::Roll(float angle)
{
    ASSERT_MSG(std::isfinite(angle), "Roll angle must be finite");
    m_roll += angle * m_rotationSpeed;
    if (m_roll > XM_2PI) m_roll -= XM_2PI;
    if (m_roll < 0.0f)   m_roll += XM_2PI;
    UpdateViewMatrix();
}

void SparkEngineCamera::SetZoom(bool enabled)
{
    // Pick FOV
    float fov = enabled ? m_zoomedFov : m_defaultFov;
    ASSERT_MSG(fov > 0.0f && fov < XM_PI, "Invalid FOV");

    // Extract aspect ratio from previous projection
    XMFLOAT4X4 m;
    XMStoreFloat4x4(&m, m_projectionMatrix);
    float aspect = m._11 / m._22;
    ASSERT_MSG(aspect > 0.0f, "Invalid aspect ratio");

    m_projectionMatrix = XMMatrixPerspectiveFovLH(
        fov, aspect, 0.1f, 1000.0f);
    // view unchanged
}