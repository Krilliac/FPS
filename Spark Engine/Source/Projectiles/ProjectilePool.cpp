// ProjectilePool.cpp
#include "ProjectilePool.h"
#include "Utils/Assert.h"
#include "..\Projectiles\Bullet.h"
#include "..\Projectiles\Rocket.h"
#include "..\Projectiles\Grenade.h"

using DirectX::XMFLOAT3;

ProjectilePool::ProjectilePool(size_t poolSize)
    : m_poolSize(poolSize)
    , m_device(nullptr)
    , m_context(nullptr)
{
    ASSERT_MSG(poolSize > 0, "ProjectilePool size must be positive");
}

ProjectilePool::~ProjectilePool()
{
    Shutdown();
}

HRESULT ProjectilePool::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ASSERT_MSG(device != nullptr, "ProjectilePool::Initialize device is null");
    ASSERT_MSG(context != nullptr, "ProjectilePool::Initialize context is null");

    m_device = device;
    m_context = context;
    CreateProjectiles();
    return S_OK;
}

void ProjectilePool::CreateProjectiles()
{
    m_projectiles.clear();
    while (!m_availableProjectiles.empty())
        m_availableProjectiles.pop();

    m_projectiles.reserve(m_poolSize);

    size_t bulletsCount = static_cast<size_t>(m_poolSize * 0.6f);
    size_t rocketsCount = static_cast<size_t>(m_poolSize * 0.3f);
    size_t grenadesCount = m_poolSize - bulletsCount - rocketsCount;

    // Helper lambda
    auto makeAndStore = [&](auto TypeFactory, size_t count) {
        for (size_t i = 0; i < count; ++i)
        {
            auto p = TypeFactory();
            ASSERT_MSG(p != nullptr, "Failed to create projectile");
            if (SUCCEEDED(p->Initialize(m_device, m_context)))
            {
                m_availableProjectiles.push(p.get());
                m_projectiles.push_back(std::move(p));
            }
        }
        };

    makeAndStore([] { return std::make_unique<Bullet>(); }, bulletsCount);
    makeAndStore([] { return std::make_unique<Rocket>(); }, rocketsCount);
    makeAndStore([] { return std::make_unique<Grenade>(); }, grenadesCount);

    ASSERT_MSG(m_projectiles.size() == m_poolSize, "Some projectiles failed to initialize");
}

void ProjectilePool::Update(float deltaTime)
{
    ASSERT_MSG(deltaTime >= 0.0f && std::isfinite(deltaTime), "Invalid deltaTime in ProjectilePool::Update");

    for (auto& up : m_projectiles)
    {
        if (up->IsActive())
        {
            up->Update(deltaTime);
            if (!up->IsActive())
                ReturnProjectile(up.get());
        }
    }
}

void ProjectilePool::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
    for (auto& up : m_projectiles)
    {
        if (up->IsActive())
            up->Render(view, projection);
    }
}

void ProjectilePool::Shutdown()
{
    m_projectiles.clear();
    std::queue<Projectile*> empty;
    std::swap(m_availableProjectiles, empty);
}

Projectile* ProjectilePool::GetProjectile()
{
    if (m_availableProjectiles.empty())
        return nullptr;
    Projectile* p = m_availableProjectiles.front();
    m_availableProjectiles.pop();
    return p;
}

void ProjectilePool::ReturnProjectile(Projectile* p)
{
    ASSERT_MSG(p != nullptr, "ReturnProjectile null pointer");
    p->Reset();
    m_availableProjectiles.push(p);
}

void ProjectilePool::FireBullet(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    ASSERT_MSG(speed >= 0.0f, "Speed must be non-negative in FireBullet");
    if (auto p = GetProjectile())
        p->Fire(pos, dir, speed);
}

void ProjectilePool::FireRocket(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    ASSERT_MSG(speed >= 0.0f, "Speed must be non-negative in FireRocket");
    if (auto p = GetProjectile())
        p->Fire(pos, dir, speed);
}

void ProjectilePool::FireGrenade(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    ASSERT_MSG(speed >= 0.0f, "Speed must be non-negative in FireGrenade");
    if (auto p = GetProjectile())
    {
        p->SetGravity(true, 1.0f);
        p->Fire(pos, dir, speed);
    }
}

void ProjectilePool::FireProjectile(ProjectileType type, const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    switch (type)
    {
    case ProjectileType::BULLET:  FireBullet(pos, dir, speed); break;
    case ProjectileType::ROCKET:  FireRocket(pos, dir, speed); break;
    case ProjectileType::GRENADE: FireGrenade(pos, dir, speed); break;
    default:
        ASSERT_MSG(false, "Unknown ProjectileType in FireProjectile");
    }
}

size_t ProjectilePool::GetActiveCount() const
{
    size_t count = 0;
    for (const auto& up : m_projectiles)
        if (up->IsActive()) ++count;
    return count;
}

size_t ProjectilePool::GetAvailableCount() const
{
    return m_availableProjectiles.size();
}