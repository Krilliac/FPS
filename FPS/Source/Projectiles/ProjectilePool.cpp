#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(size_t poolSize)
    : m_poolSize(poolSize)
    , m_device(nullptr)
    , m_context(nullptr)
{
}

ProjectilePool::~ProjectilePool()
{
    Shutdown();
}

HRESULT ProjectilePool::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;
    
    CreateProjectiles();
    
    return S_OK;
}

void ProjectilePool::Update(float deltaTime)
{
    // Update all projectiles and return inactive ones to pool
    for (auto& projectile : m_projectiles)
    {
        if (projectile->IsActive())
        {
            projectile->Update(deltaTime);
            
            // Check if projectile became inactive and return to pool
            if (!projectile->IsActive())
            {
                ReturnProjectile(projectile.get());
            }
        }
    }
}

void ProjectilePool::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    for (auto& projectile : m_projectiles)
    {
        if (projectile->IsActive())
        {
            projectile->Render(view, projection);
        }
    }
}

void ProjectilePool::Shutdown()
{
    m_projectiles.clear();
    while (!m_availableProjectiles.empty())
    {
        m_availableProjectiles.pop();
    }
}

Projectile* ProjectilePool::GetProjectile()
{
    if (m_availableProjectiles.empty())
    {
        return nullptr; // Pool exhausted
    }
    
    Projectile* projectile = m_availableProjectiles.front();
    m_availableProjectiles.pop();
    return projectile;
}

void ProjectilePool::ReturnProjectile(Projectile* projectile)
{
    if (projectile)
    {
        projectile->Reset();
        m_availableProjectiles.push(projectile);
    }
}

void ProjectilePool::FireProjectile(ProjectileType type, const XMFLOAT3& startPosition, 
                                  const XMFLOAT3& direction, float speed)
{
    switch (type)
    {
    case ProjectileType::BULLET:
        FireBullet(startPosition, direction, speed);
        break;
    case ProjectileType::ROCKET:
        FireRocket(startPosition, direction, speed);
        break;
    case ProjectileType::GRENADE:
        FireGrenade(startPosition, direction, speed);
        break;
    }
}

void ProjectilePool::FireBullet(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed)
{
    Projectile* projectile = GetProjectile();
    if (projectile)
    {
        projectile->Fire(startPosition, direction, speed);
    }
}

void ProjectilePool::FireRocket(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed)
{
    Projectile* projectile = GetProjectile();
    if (projectile)
    {
        projectile->Fire(startPosition, direction, speed);
    }
}

void ProjectilePool::FireGrenade(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed)
{
    Projectile* projectile = GetProjectile();
    if (projectile)
    {
        projectile->SetGravity(true, 1.0f);
        projectile->Fire(startPosition, direction, speed);
    }
}

size_t ProjectilePool::GetActiveCount() const
{
    size_t activeCount = 0;
    for (const auto& projectile : m_projectiles)
    {
        if (projectile->IsActive())
        {
            activeCount++;
        }
    }
    return activeCount;
}

void ProjectilePool::ClearAll()
{
    for (auto& projectile : m_projectiles)
    {
        if (projectile->IsActive())
        {
            projectile->Deactivate();
            ReturnProjectile(projectile.get());
        }
    }
}

void ProjectilePool::CreateProjectiles()
{
    m_projectiles.reserve(m_poolSize);
    
    // Create a mix of different projectile types
    size_t bulletsCount = static_cast<size_t>(m_poolSize * 0.6f);  // 60% bullets
    size_t rocketsCount = static_cast<size_t>(m_poolSize * 0.3f);  // 30% rockets
    size_t grenadesCount = static_cast<size_t>(m_poolSize * 0.1f); // 10% grenades
    
    // Create bullets
    for (size_t i = 0; i < bulletsCount; ++i)
    {
        auto projectile = std::make_unique<Bullet>();
        if (SUCCEEDED(projectile->Initialize(m_device, m_context)))
        {
            m_availableProjectiles.push(projectile.get());
            m_projectiles.push_back(std::move(projectile));
        }
    }
    
    // Create rockets
    for (size_t i = 0; i < rocketsCount; ++i)
    {
        auto projectile = std::make_unique<Rocket>();
        if (SUCCEEDED(projectile->Initialize(m_device, m_context)))
        {
            m_availableProjectiles.push(projectile.get());
            m_projectiles.push_back(std::move(projectile));
        }
    }
    
    // Create grenades
    for (size_t i = 0; i < grenadesCount; ++i)
    {
        auto projectile = std::make_unique<Grenade>();
        if (SUCCEEDED(projectile->Initialize(m_device, m_context)))
        {
            m_availableProjectiles.push(projectile.get());
            m_projectiles.push_back(std::move(projectile));
        }
    }
}

Projectile* ProjectilePool::CreateProjectileOfType(ProjectileType type)
{
    switch (type)
    {
    case ProjectileType::BULLET:
        return new Bullet();
    case ProjectileType::ROCKET:
        return new Rocket();
    case ProjectileType::GRENADE:
        return new Grenade();
    default:
        return new Bullet();
    }
}
