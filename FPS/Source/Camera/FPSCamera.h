#pragma once
#include "..\Core\framework.h"

class FPSCamera
{
private:
    XMFLOAT3 m_position;
    XMFLOAT3 m_forward;
    XMFLOAT3 m_right;
    XMFLOAT3 m_up;
    float m_pitch;
    float m_yaw;
    float m_roll;
    
    XMMATRIX m_viewMatrix;
    XMMATRIX m_projectionMatrix;
    
    float m_moveSpeed;
    float m_rotationSpeed;

public:
    FPSCamera();
    ~FPSCamera();

    void Initialize(float aspectRatio);
    void Update(float deltaTime);
    void UpdateViewMatrix();
    
    // Movement
    void MoveForward(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    
    // Rotation
    void Pitch(float angle);
    void Yaw(float angle);
    
    // Accessors
    const XMMATRIX& GetViewMatrix() const { return m_viewMatrix; }
    const XMMATRIX& GetProjectionMatrix() const { return m_projectionMatrix; }
    const XMFLOAT3& GetPosition() const { return m_position; }
    const XMFLOAT3& GetForward() const { return m_forward; }
    
    void SetPosition(const XMFLOAT3& position);
    void SetMoveSpeed(float speed) { m_moveSpeed = speed; }
    void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
};
