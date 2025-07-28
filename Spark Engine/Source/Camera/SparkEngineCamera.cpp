#include "SparkEngineCamera.h"
#include <DirectXMath.h>
#include <dxgi.h>

using namespace DirectX;

void SparkEngineCamera::Initialize(float aspectRatio)
{
    m_projectionMatrix = XMMatrixPerspectiveFovLH(
        m_defaultFov, aspectRatio, 0.1f, 1000.0f);
    UpdateViewMatrix();
}

void SparkEngineCamera::UpdateViewMatrix()
{
    XMMATRIX rot = XMMatrixRotationRollPitchYaw(
        m_pitch, m_yaw, m_roll);

    XMVECTOR fb = XMVector3TransformCoord(
        XMVectorSet(0, 0, 1, 0), rot);
    XMVECTOR rb = XMVector3TransformCoord(
        XMVectorSet(1, 0, 0, 0), rot);
    XMVECTOR ub = XMVector3TransformCoord(
        XMVectorSet(0, 1, 0, 0), rot);

    XMStoreFloat3(&m_forward, fb);
    XMStoreFloat3(&m_right, rb);
    XMStoreFloat3(&m_up, ub);

    XMVECTOR pos = XMLoadFloat3(&m_position);
    m_viewMatrix = XMMatrixLookAtLH(pos, pos + fb, ub);
}

void SparkEngineCamera::MoveForward(float amount)
{
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR f = XMLoadFloat3(&m_forward);
    p += f * amount;
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::MoveRight(float amount)
{
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR r = XMLoadFloat3(&m_right);
    p += r * amount;
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::MoveUp(float amount)
{
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR u = XMLoadFloat3(&m_up);
    p += u * amount;
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::Pitch(float angle)
{
    m_pitch = std::clamp(
        m_pitch + angle * m_rotationSpeed,
        -XM_PIDIV2 + 0.01f,
        XM_PIDIV2 - 0.01f);
    UpdateViewMatrix();
}

void SparkEngineCamera::Yaw(float angle)
{
    m_yaw += angle * m_rotationSpeed;
    if (m_yaw > XM_2PI) m_yaw -= XM_2PI;
    if (m_yaw < 0.0f)   m_yaw += XM_2PI;
    UpdateViewMatrix();
}

void SparkEngineCamera::SetZoom(bool enabled)
{
    float fov = enabled ? m_zoomedFov : m_defaultFov;
    // extract aspect ratio from previous projection
    XMFLOAT4X4 p; XMStoreFloat4x4(&p, m_projectionMatrix);
    float aspect = p._11 / p._22;
    m_projectionMatrix = XMMatrixPerspectiveFovLH(
        fov, aspect, 0.1f, 1000.0f);
}
