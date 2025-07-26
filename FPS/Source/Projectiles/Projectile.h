#pragma once  
#include "..\Game\GameObject.h"  
#include "..\Physics\CollisionSystem.h"  

class Projectile : public GameObject  
{  
private:  
    XMFLOAT3 m_velocity;  
    float m_speed;  
    float m_lifeTime;  
    float m_maxLifeTime;  
    float m_damage;  
    bool m_active;  

    // Physics properties  
    BoundingSphere m_boundingSphere;  
    bool m_hasGravity;  
    float m_gravityScale;  

public:  
    Projectile();  
    ~Projectile();  

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;  
    virtual void Update(float deltaTime) = 0;  
    void Render(const XMMATRIX& view, const XMMATRIX& projection) override;  

    // Projectile specific methods  
    void Fire(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed);  
    void Deactivate();  
    void Reset();  

    // Collision  
    void OnHit(GameObject* target) override; // Ensure this matches the pure virtual function in GameObject  
    void OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal) override;  

    // Physics  
    void SetGravity(bool enabled, float scale = 1.0f);  
    void ApplyForce(const XMFLOAT3& force);  

    // Accessors  
    bool IsActive() const { return m_active; }  
    float GetDamage() const { return m_damage; }  
    const XMFLOAT3& GetVelocity() const { return m_velocity; }  
    const BoundingSphere& GetBoundingSphere() const { return m_boundingSphere; }  

    void SetDamage(float damage) { m_damage = damage; }  
    void SetLifeTime(float lifeTime) { m_maxLifeTime = lifeTime; }  

protected:  
    void CreateMesh() override;  
    void CheckCollisions();  
    void UpdatePhysics(float deltaTime);  
    void UpdateBoundingSphere();  
};

// Specialized projectile types  
class Bullet : public Projectile  
{  
public:  
    Bullet();  
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;  
};  

class Rocket : public Projectile  
{  
private:  
    float m_explosionRadius;  
    bool m_hasExploded;  

public:  
    Rocket();  
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;  
    void OnHit(GameObject* target) override;  
    void OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal) override;  

private:  
    void Explode(const XMFLOAT3& position);  
};  

class Grenade : public Projectile  
{  
private:  
    float m_fuseTime;  
    float m_explosionRadius;  
    bool m_hasExploded;  

public:  
    Grenade();  
    void Update(float deltaTime) override;  
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context) override;  

private:  
    void Explode();  
};
