#include "FPSCamera.h"

FPSCamera::FPSCamera()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_forward(0.0f, 0.0f, 1.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_pitch(0.0f)
    , m_yaw(0.0f)
    , m_roll(0.0f)
    , m_moveSpeed(10.0f)
    , m_rotationSpeed(2.0f)
{
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Initialize(float aspectRatio)
{
    m_projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, aspectRatio, 0.1f, 1000.0f);
    UpdateViewMatrix();
}

void FPSCamera::Update(float deltaTime)
{
    // Camera updates are handled by input system
    // This can be extended for smooth camera interpolation
}

void FPSCamera::UpdateViewMatrix()
{
    // Create rotation matrix from pitch and yaw
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);
    
    // Transform basis vectors
    XMVECTOR forwardBase = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR rightBase = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR upBase = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMStoreFloat3(&m_forward, XMVector3TransformCoord(forwardBase, rotationMatrix));
    XMStoreFloat3(&m_right, XMVector3TransformCoord(rightBase, rotationMatrix));
    XMStoreFloat3(&m_up, XMVector3TransformCoord(upBase, rotationMatrix));

    // Create view matrix
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR forward = XMLoadFloat3(&m_forward);
    XMVECTOR up = XMLoadFloat3(&m_up);

    m_viewMatrix = XMMatrixLookAtLH(pos, pos + forward, up);
}

void FPSCamera::MoveForward(float distance)
{
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR forward = XMLoadFloat3(&m_forward);
    pos += forward * distance;
    XMStoreFloat3(&m_position, pos);
    UpdateViewMatrix();
}

void FPSCamera::MoveRight(float distance)
{
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR right = XMLoadFloat3(&m_right);
    pos += right * distance;
    XMStoreFloat3(&m_position, pos);
    UpdateViewMatrix();
}

void FPSCamera::MoveUp(float distance)
{
    XMVECTOR pos = XMLoadFloat3(&m_position);
    XMVECTOR up = XMLoadFloat3(&m_up);
    pos += up * distance;
    XMStoreFloat3(&m_position, pos);
    UpdateViewMatrix();
}

void FPSCamera::Pitch(float angle)
{
    m_pitch += angle;
    
    // Clamp pitch to prevent over-rotation
    const float maxPitch = XM_PIDIV2 - 0.1f;
    if (m_pitch > maxPitch) m_pitch = maxPitch;
    if (m_pitch < -maxPitch) m_pitch = -maxPitch;
    
    UpdateViewMatrix();
}

void FPSCamera::Yaw(float angle)
{
    m_yaw += angle;
    
    // Keep yaw in valid range
    if (m_yaw > XM_2PI) m_yaw -= XM_2PI;
    if (m_yaw < 0) m_yaw += XM_2PI;
    
    UpdateViewMatrix();
}

void FPSCamera::SetPosition(const XMFLOAT3& position)
{
    m_position = position;
    UpdateViewMatrix();
}
