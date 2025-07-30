﻿// Projectile.cpp
#include "Projectile.h"
#include "Utils/Assert.h"
#include "..\Utils\MathUtils.h"
#include <DirectXMath.h>

using namespace DirectX;

Projectile::Projectile()
    : m_velocity{ 0,0,0 }
    , m_speed(50.0f)
    , m_lifeTime(0.0f)
    , m_maxLifeTime(5.0f)
    , m_damage(25.0f)
    , m_active(false)
    , m_boundingSphere(GetPosition(), 0.1f)
    , m_hasGravity(false)
    , m_gravityScale(1.0f)
{
    // Base GameObject scale
    XMFLOAT3 scale{ 0.1f,0.1f,0.3f };
    ASSERT_MSG(scale.x > 0 && scale.y > 0 && scale.z > 0, "Scale must be positive");
    SetScale(scale);
}

Projectile::~Projectile() = default;

HRESULT Projectile::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT_MSG(device != nullptr, "Device is null");
    ASSERT_MSG(context != nullptr, "Context is null");

    HRESULT hr = GameObject::Initialize(device, context);
    ASSERT_MSG(SUCCEEDED(hr), "GameObject::Initialize failed in Projectile");
    if (FAILED(hr)) return hr;

    UpdateBoundingSphere();
    return S_OK;
}

void Projectile::Update(float deltaTime)
{
    ASSERT_MSG(deltaTime >= 0 && std::isfinite(deltaTime), "Invalid deltaTime");
    if (!m_active) return;

    // Physics integration
    UpdatePhysics(deltaTime);

    // Move
    XMFLOAT3 delta{ m_velocity.x * deltaTime,
                    m_velocity.y * deltaTime,
                    m_velocity.z * deltaTime };
    Translate(delta);

    // Lifetime
    m_lifeTime += deltaTime;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Deactivate();
        return;
    }

    // Collision
    CheckCollisions();

    // Update transform
    GameObject::Update(deltaTime);

    // Update bounding volume
    UpdateBoundingSphere();
}

void Projectile::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    if (!m_active) return;
    GameObject::Render(view, projection);
}

void Projectile::Fire(const XMFLOAT3& startPosition,
    const XMFLOAT3& direction,
    float speed)
{
    ASSERT_MSG(speed >= 0, "Speed must be non-negative");
    SetPosition(startPosition);
    m_speed = speed;

    XMVECTOR dirV = XMVector3Normalize(XMLoadFloat3(&direction));
    XMVECTOR velV = XMVectorScale(dirV, speed);
    XMStoreFloat3(&m_velocity, velV);

    m_lifeTime = 0.0f;
    m_active = true;
    SetActive(true);
    SetVisible(true);

    UpdateBoundingSphere();
}

void Projectile::Deactivate()
{
    m_active = false;
    SetActive(false);
    SetVisible(false);
    m_lifeTime = 0.0f;
    m_velocity = XMFLOAT3{ 0,0,0 };
}

void Projectile::Reset()
{
    Deactivate();
    SetPosition(XMFLOAT3{ 0,0,0 });
    SetRotation(XMFLOAT3{ 0,0,0 });
}

void Projectile::OnHit(GameObject* target)
{
    ASSERT_MSG(target != nullptr, "OnHit target null");
    Deactivate();
}

void Projectile::OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal)
{
    // normal not validated
    Deactivate();
}

void Projectile::SetGravity(bool enabled, float scale)
{
    ASSERT_MSG(scale >= 0, "Gravity scale must be non-negative");
    m_hasGravity = enabled;
    m_gravityScale = scale;
}

void Projectile::ApplyForce(const XMFLOAT3& force)
{
    XMVECTOR v = XMLoadFloat3(&m_velocity);
    XMVECTOR f = XMLoadFloat3(&force);
    XMVECTOR sum = XMVectorAdd(v, f);
    XMStoreFloat3(&m_velocity, sum);
}

void Projectile::CreateMesh()
{
    if (m_mesh)
        m_mesh->CreateSphere(0.1f, 8, 8);
}

void Projectile::CheckCollisions()
{
    // Example: ground plane at y=0
    if (GetPosition().y < 0.0f)
    {
        OnHitWorld(GetPosition(), XMFLOAT3{ 0,1,0 });
    }
}

void Projectile::UpdatePhysics(float deltaTime)
{
    if (m_hasGravity)
        m_velocity.y += -9.8f * m_gravityScale * deltaTime;

    // Simple drag
    XMVECTOR v = XMLoadFloat3(&m_velocity);
    v = XMVectorScale(v, 0.98f);
    XMStoreFloat3(&m_velocity, v);
}

void Projectile::UpdateBoundingSphere()
{
    m_boundingSphere.Center = GetPosition();
    // radius remains unchanged
}