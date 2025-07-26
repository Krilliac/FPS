#include "Projectile.h"
#include "..\Utils\MathUtils.h"

Projectile::Projectile()
    : m_velocity(0.0f, 0.0f, 0.0f)
    , m_speed(50.0f)
    , m_lifeTime(0.0f)
    , m_maxLifeTime(5.0f)
    , m_damage(25.0f)
    , m_active(false)
    , m_hasGravity(false)
    , m_gravityScale(1.0f)
{
    SetScale(XMFLOAT3(0.1f, 0.1f, 0.3f)); // Small projectile shape
    m_boundingSphere = BoundingSphere(m_position, 0.1f);
}

Projectile::~Projectile()
{
}

HRESULT Projectile::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return GameObject::Initialize(device, context);
}

void Projectile::Update(float deltaTime)
{
    if (!m_active) return;
    
    // Update physics
    UpdatePhysics(deltaTime);
    
    // Update position based on velocity
    XMFLOAT3 movement;
    movement.x = m_velocity.x * deltaTime;
    movement.y = m_velocity.y * deltaTime;
    movement.z = m_velocity.z * deltaTime;
    
    Translate(movement);
    
    // Update bounding sphere
    UpdateBoundingSphere();
    
    // Update lifetime
    m_lifeTime += deltaTime;
    if (m_lifeTime >= m_maxLifeTime)
    {
        Deactivate();
        return;
    }
    
    // Check for collisions
    CheckCollisions();
    
    GameObject::Update(deltaTime);
}

void Projectile::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    if (!m_active) return;
    
    GameObject::Render(view, projection);
}

void Projectile::Fire(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed)
{
    SetPosition(startPosition);
    m_speed = speed;
    
    // Normalize direction and set velocity
    XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&direction));
    XMVECTOR vel = dir * m_speed;
    XMStoreFloat3(&m_velocity, vel);
    
    // Reset state
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
    m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Projectile::Reset()
{
    Deactivate();
    SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
    SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
}

void Projectile::OnHit(GameObject* target)
{
    // Handle collision with target object
    // Apply damage, create effects, etc.
    Deactivate();
}

void Projectile::OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal)
{
    // Handle collision with world geometry
    // Create impact effects, bounce, etc.
    Deactivate();
}

void Projectile::SetGravity(bool enabled, float scale)
{
    m_hasGravity = enabled;
    m_gravityScale = scale;
}

void Projectile::ApplyForce(const XMFLOAT3& force)
{
    XMVECTOR vel = XMLoadFloat3(&m_velocity);
    XMVECTOR forceVec = XMLoadFloat3(&force);
    vel += forceVec;
    XMStoreFloat3(&m_velocity, vel);
}

void Projectile::CreateMesh()
{
    if (m_mesh)
    {
        // Create a small sphere for the projectile
        m_mesh->CreateSphere(0.1f, 8, 8);
    }
}

void Projectile::CheckCollisions()
{
    // TODO: Implement collision detection with world geometry and other objects
    // This would typically involve checking against a collision manager or world
    
    // Simple bounds check for now
    if (m_position.y < -10.0f) // Below ground level
    {
        OnHitWorld(m_position, XMFLOAT3(0.0f, 1.0f, 0.0f));
    }
}

void Projectile::UpdatePhysics(float deltaTime)
{
    if (m_hasGravity)
    {
        // Apply gravity
        const float gravity = -9.8f * m_gravityScale;
        m_velocity.y += gravity * deltaTime;
    }
    
    // Apply air resistance (simple drag)
    const float dragCoefficient = 0.98f;
    XMVECTOR vel = XMLoadFloat3(&m_velocity);
    vel *= dragCoefficient;
    XMStoreFloat3(&m_velocity, vel);
}

void Projectile::UpdateBoundingSphere()
{
    m_boundingSphere.Center = m_position;
    // Radius stays the same unless projectile changes size
}

// Bullet Implementation
Bullet::Bullet()
{
    m_damage = 15.0f;
    m_speed = 100.0f;
    m_maxLifeTime = 3.0f;
    SetScale(XMFLOAT3(0.05f, 0.05f, 0.2f));
}

HRESULT Bullet::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return Projectile::Initialize(device, context);
}

// Rocket Implementation
Rocket::Rocket()
    : m_explosionRadius(5.0f)
    , m_hasExploded(false)
{
    m_damage = 75.0f;
    m_speed = 30.0f;
    m_maxLifeTime = 10.0f;
    SetGravity(true, 0.3f); // Slight gravity effect
    SetScale(XMFLOAT3(0.2f, 0.2f, 0.8f));
}

HRESULT Rocket::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return Projectile::Initialize(device, context);
}

void Rocket::OnHit(GameObject* target)
{
    if (!m_hasExploded)
    {
        Explode(m_position);
    }
}

void Rocket::OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal)
{
    if (!m_hasExploded)
    {
        Explode(hitPoint);
    }
}

void Rocket::Explode(const XMFLOAT3& position)
{
    m_hasExploded = true;
    
    // TODO: Create explosion effects
    // TODO: Apply area damage to nearby objects
    
    Deactivate();
}

// Grenade Implementation
Grenade::Grenade()
    : m_fuseTime(3.0f)
    , m_explosionRadius(8.0f)
    , m_hasExploded(false)
{
    m_damage = 100.0f;
    m_speed = 15.0f;
    m_maxLifeTime = 5.0f;
    SetGravity(true, 1.0f); // Full gravity
    SetScale(XMFLOAT3(0.3f, 0.3f, 0.3f));
}

void Grenade::Update(float deltaTime)
{
    if (!m_active) return;
    
    // Check fuse timer
    if (m_lifeTime >= m_fuseTime && !m_hasExploded)
    {
        Explode();
        return;
    }
    
    Projectile::Update(deltaTime);
}

HRESULT Grenade::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return Projectile::Initialize(device, context);
}

void Grenade::Explode()
{
    m_hasExploded = true;
    
    // TODO: Create explosion effects
    // TODO: Apply area damage to nearby objects
    
    Deactivate();
}
