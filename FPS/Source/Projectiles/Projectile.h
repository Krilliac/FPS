#pragma once

#include "..\Core\framework.h"            // XMFLOAT3, XMMATRIX
#include "..\Physics\CollisionSystem.h"  // BoundingSphere
#include "..\Game\GameObject.h"

class Projectile : public GameObject
{
protected:  // allow derived classes to access these
    // Motion
    XMFLOAT3 m_velocity;
    float    m_speed;
    float    m_lifeTime;
    float    m_maxLifeTime;
    float    m_damage;
    bool     m_active;

    // Physics
    BoundingSphere m_boundingSphere;
    bool           m_hasGravity;
    float          m_gravityScale;

public:
    Projectile();
    virtual ~Projectile();

    // GameObject overrides
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;
    void    Update(float deltaTime) override;
    void    Render(const XMMATRIX& view, const XMMATRIX& projection) override;

    // Projectile actions
    void Fire(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed);
    void Deactivate();
    void Reset();

    // Collision callbacks
    void OnHit(GameObject* target) override;
    void OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal) override;

    // Physics controls
    void SetGravity(bool enabled, float scale = 1.0f);
    void ApplyForce(const XMFLOAT3& force);

    // Accessors
    bool                IsActive() const { return m_active; }
    float               GetDamage() const { return m_damage; }
    const XMFLOAT3& GetVelocity() const { return m_velocity; }
    const BoundingSphere& GetBoundingSphere() const { return m_boundingSphere; }

    void SetDamage(float damage) { m_damage = damage; }
    void SetLifeTime(float lifeTime) { m_maxLifeTime = lifeTime; }

protected:
    // Internal updates
    void CreateMesh() override;
    void CheckCollisions();
    void UpdatePhysics(float deltaTime);
    void UpdateBoundingSphere();
};