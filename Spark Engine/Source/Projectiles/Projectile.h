// Projectile.h
#pragma once

#include "..\Core\framework.h"            // XMFLOAT3, XMMATRIX, HRESULT
#include "..\Physics\CollisionSystem.h"   // BoundingSphere
#include "..\Game\GameObject.h"
#include "Utils/Assert.h"

class Projectile : public GameObject
{
protected:
    // Motion
    DirectX::XMFLOAT3    m_velocity;
    float                m_speed;
    float                m_lifeTime;
    float                m_maxLifeTime;
    float                m_damage;
    bool                 m_active;

    // Physics
    BoundingSphere       m_boundingSphere;
    bool                 m_hasGravity;
    float                m_gravityScale;

public:
    Projectile();
    ~Projectile() override;

    // GameObject overrides
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;
    void    Update(float deltaTime) override;
    void    Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) override;

    // Projectile actions
    void Fire(const DirectX::XMFLOAT3& startPosition,
        const DirectX::XMFLOAT3& direction,
        float speed);
    void Deactivate();
    void Reset();

    // Collision callbacks
    void OnHit(GameObject* target) override;
    void OnHitWorld(const DirectX::XMFLOAT3& hitPoint,
        const DirectX::XMFLOAT3& normal) override;

    // Physics controls
    void SetGravity(bool enabled, float scale = 1.0f);
    void ApplyForce(const DirectX::XMFLOAT3& force);

    // Accessors
    bool                IsActive() const { return m_active; }
    float               GetDamage() const { return m_damage; }
    const DirectX::XMFLOAT3& GetVelocity() const { return m_velocity; }
    const BoundingSphere& GetBoundingSphere() const { return m_boundingSphere; }

    void SetDamage(float damage) { ASSERT_MSG(damage >= 0, "Damage must be non-negative"); m_damage = damage; }
    void SetLifeTime(float lifeTime) { ASSERT_MSG(lifeTime > 0, "LifeTime must be positive"); m_maxLifeTime = lifeTime; }

protected:
    // Internal helpers
    void CreateMesh() override;
    void CheckCollisions();
    void UpdatePhysics(float deltaTime);
    void UpdateBoundingSphere();
};