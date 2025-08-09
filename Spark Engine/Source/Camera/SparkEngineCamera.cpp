#include "SparkEngineCamera.h"
#include "Utils/Assert.h"
#include "../Utils/ConsoleProcessManager.h"
#include <DirectXMath.h>
#include <cmath>
#include <iostream>

using namespace DirectX;

// **FIXED: Rate-limited logging for Camera to prevent console spam**
#define LOG_TO_CONSOLE_RATE_LIMITED(msg, type) \
    do { \
        static auto lastLogTime = std::chrono::steady_clock::now(); \
        static int logCounter = 0; \
        auto now = std::chrono::steady_clock::now(); \
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastLogTime).count(); \
        if (elapsed >= 10 || logCounter < 1) { \
            Spark::ConsoleProcessManager::GetInstance().Log(msg, type); \
            if (elapsed >= 10) { \
                lastLogTime = now; \
                logCounter = 0; \
            } else { \
                logCounter++; \
            } \
        } \
    } while(0)

// Use rate-limited logging for most messages, immediate for critical ones
#define LOG_TO_CONSOLE(msg, type) LOG_TO_CONSOLE_RATE_LIMITED(msg, type)
#define LOG_TO_CONSOLE_IMMEDIATE(msg, type) Spark::ConsoleProcessManager::GetInstance().Log(msg, type)

void SparkEngineCamera::Initialize(float aspectRatio) {
    LOG_TO_CONSOLE_IMMEDIATE(L"SparkEngineCamera::Initialize called. aspectRatio=" + std::to_wstring(aspectRatio), L"OPERATION");
    ASSERT_MSG(aspectRatio > 0.0f, "Aspect ratio must be positive");
    m_projectionMatrix = XMMatrixPerspectiveFovLH(m_defaultFov, aspectRatio, 0.1f, 1000.0f);
    UpdateViewMatrix();
    LOG_TO_CONSOLE_IMMEDIATE(L"Camera initialized with aspect ratio.", L"INFO");
}

void SparkEngineCamera::UpdateViewMatrix() {
    // **FIXED: Remove per-frame logging completely**
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
}

void SparkEngineCamera::Update(float deltaTime) {
    // **FIXED: Remove per-frame logging completely**
    ASSERT_MSG(deltaTime >= 0.0f && std::isfinite(deltaTime), "Camera Update deltaTime must be non-negative and finite");
    UpdateViewMatrix();
}

void SparkEngineCamera::MoveForward(float amount) {
    // **FIXED: Remove per-frame logging completely**
    ASSERT_MSG(std::isfinite(amount), "Move amount must be finite");
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR f = XMLoadFloat3(&m_forward);
    p = XMVectorAdd(p, XMVectorScale(f, amount * m_moveSpeed));
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::MoveRight(float amount) {
    // **FIXED: Remove per-frame logging completely**
    ASSERT_MSG(std::isfinite(amount), "Move amount must be finite");
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR r = XMLoadFloat3(&m_right);
    p = XMVectorAdd(p, XMVectorScale(r, amount * m_moveSpeed));
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::MoveUp(float amount) {
    // **FIXED: Remove per-frame logging completely**
    ASSERT_MSG(std::isfinite(amount), "Move amount must be finite");
    XMVECTOR p = XMLoadFloat3(&m_position);
    XMVECTOR u = XMLoadFloat3(&m_up);
    p = XMVectorAdd(p, XMVectorScale(u, amount * m_moveSpeed));
    XMStoreFloat3(&m_position, p);
    UpdateViewMatrix();
}

void SparkEngineCamera::Pitch(float angle) {
    // **FIXED: Rate-limited logging for camera rotation**
    LOG_TO_CONSOLE(L"SparkEngineCamera::Pitch called. angle=" + std::to_wstring(angle), L"OPERATION");
    ASSERT_MSG(std::isfinite(angle), "Angle must be finite");
    m_pitch = std::clamp(
        m_pitch + angle * m_rotationSpeed,
        -XM_PIDIV2 + 0.01f,
        XM_PIDIV2 - 0.01f);
    UpdateViewMatrix();
}

void SparkEngineCamera::Yaw(float angle) {
    // **FIXED: Rate-limited logging for camera rotation**
    LOG_TO_CONSOLE(L"SparkEngineCamera::Yaw called. angle=" + std::to_wstring(angle), L"OPERATION");
    ASSERT_MSG(std::isfinite(angle), "Angle must be finite");
    m_yaw += angle * m_rotationSpeed;
    if (m_yaw > XM_2PI) m_yaw -= XM_2PI;
    if (m_yaw < 0.0f)   m_yaw += XM_2PI;
    UpdateViewMatrix();
}

void SparkEngineCamera::Roll(float angle) {
    LOG_TO_CONSOLE(L"SparkEngineCamera::Roll called. angle=" + std::to_wstring(angle), L"OPERATION");
    ASSERT_MSG(std::isfinite(angle), "Roll angle must be finite");
    m_roll += angle * m_rotationSpeed;
    if (m_roll > XM_2PI) m_roll -= XM_2PI;
    if (m_roll < 0.0f)   m_roll += XM_2PI;
    UpdateViewMatrix();
}

void SparkEngineCamera::SetZoom(bool enabled) {
    LOG_TO_CONSOLE(L"SparkEngineCamera::SetZoom called. enabled=" + std::to_wstring(enabled), L"OPERATION");
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
    LOG_TO_CONSOLE(L"Camera zoom set.", L"INFO");
}