// ProjectilePool.cpp
#include "ProjectilePool.h"
#include "Utils/Assert.h"
#include "../Utils/ConsoleProcessManager.h"
#include <iostream>

using namespace DirectX;

// Helper macro for logging to external console
#define LOG_TO_CONSOLE(msg, type) Spark::ConsoleProcessManager::GetInstance().Log(msg, type)

ProjectilePool::ProjectilePool(size_t poolSize)
    : m_poolSize(poolSize)
{
    LOG_TO_CONSOLE(L"ProjectilePool constructed with size " + std::to_wstring(poolSize), L"INFO");
    ASSERT_MSG(poolSize > 0, "ProjectilePool size must be positive");
    m_projectiles.reserve(poolSize);
}

ProjectilePool::~ProjectilePool()
{
    LOG_TO_CONSOLE(L"ProjectilePool destructor called.", L"INFO");
    Shutdown();
}

HRESULT ProjectilePool::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    LOG_TO_CONSOLE(L"ProjectilePool::Initialize called.", L"OPERATION");
    ASSERT(device != nullptr);
    ASSERT(context != nullptr);

    m_device = device;
    m_context = context;

    // Create projectiles based on pool size distribution
    size_t bulletsCount = m_poolSize / 2;     // 50% bullets
    size_t rocketsCount = m_poolSize / 4;     // 25% rockets
    size_t grenadesCount = m_poolSize - bulletsCount - rocketsCount; // remainder

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

    LOG_TO_CONSOLE(L"ProjectilePool created " + std::to_wstring(m_projectiles.size()) + L" projectiles.", L"INFO");
    return S_OK;
}

void ProjectilePool::Update(float deltaTime)
{
    LOG_TO_CONSOLE(L"ProjectilePool::Update called. deltaTime=" + std::to_wstring(deltaTime), L"OPERATION");
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
    LOG_TO_CONSOLE(L"ProjectilePool update complete.", L"INFO");
}

void ProjectilePool::Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection)
{
    LOG_TO_CONSOLE(L"ProjectilePool::Render called.", L"OPERATION");
    for (auto& up : m_projectiles)
    {
        if (up->IsActive())
            up->Render(view, projection);
    }
    LOG_TO_CONSOLE(L"ProjectilePool render complete.", L"INFO");
}

void ProjectilePool::Shutdown()
{
    LOG_TO_CONSOLE(L"ProjectilePool::Shutdown called.", L"OPERATION");

    m_projectiles.clear();
    std::queue<Projectile*> empty;
    std::swap(m_availableProjectiles, empty);

    LOG_TO_CONSOLE(L"ProjectilePool shutdown complete.", L"INFO");
}

Projectile* ProjectilePool::GetProjectile()
{
    LOG_TO_CONSOLE(L"ProjectilePool::GetProjectile called.", L"OPERATION");
    if (m_availableProjectiles.empty()) {
        LOG_TO_CONSOLE(L"ProjectilePool: No available projectiles!", L"WARNING");
        return nullptr;
    }
    Projectile* p = m_availableProjectiles.front();
    m_availableProjectiles.pop();
    LOG_TO_CONSOLE(L"ProjectilePool: Got projectile.", L"INFO");
    return p;
}

void ProjectilePool::ReturnProjectile(Projectile* p)
{
    LOG_TO_CONSOLE(L"ProjectilePool::ReturnProjectile called.", L"OPERATION");
    ASSERT(p != nullptr);
    if (p)
    {
        p->SetActive(false);
        m_availableProjectiles.push(p);
        LOG_TO_CONSOLE(L"ProjectilePool: Projectile returned to pool.", L"INFO");
    }
}

void ProjectilePool::FireBullet(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    LOG_TO_CONSOLE(L"ProjectilePool::FireBullet called. speed=" + std::to_wstring(speed), L"OPERATION");
    ASSERT_MSG(speed >= 0.0f, "Speed must be non-negative in FireBullet");
    if (auto p = GetProjectile()) {
        p->Fire(pos, dir, speed);
    }
    LOG_TO_CONSOLE(L"ProjectilePool: Bullet fired.", L"INFO");
}

void ProjectilePool::FireRocket(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    LOG_TO_CONSOLE(L"ProjectilePool::FireRocket called. speed=" + std::to_wstring(speed), L"OPERATION");
    ASSERT_MSG(speed >= 0.0f, "Speed must be non-negative in FireRocket");
    if (auto p = GetProjectile()) {
        p->Fire(pos, dir, speed);
    }
    LOG_TO_CONSOLE(L"ProjectilePool: Rocket fired.", L"INFO");
}

void ProjectilePool::FireGrenade(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    LOG_TO_CONSOLE(L"ProjectilePool::FireGrenade called. speed=" + std::to_wstring(speed), L"OPERATION");
    ASSERT_MSG(speed >= 0.0f, "Speed must be non-negative in FireGrenade");
    if (auto p = GetProjectile()) {
        p->SetGravity(true, 1.0f);
        p->Fire(pos, dir, speed);
    }
    LOG_TO_CONSOLE(L"ProjectilePool: Grenade fired.", L"INFO");
}

void ProjectilePool::FireProjectile(ProjectileType type, const XMFLOAT3& pos, const XMFLOAT3& dir, float speed)
{
    LOG_TO_CONSOLE(L"ProjectilePool::FireProjectile called. type=" + std::to_wstring(static_cast<int>(type)) + L" speed=" + std::to_wstring(speed), L"OPERATION");
    switch (type)
    {
    case ProjectileType::BULLET:  FireBullet(pos, dir, speed); break;
    case ProjectileType::ROCKET:  FireRocket(pos, dir, speed); break;
    case ProjectileType::GRENADE: FireGrenade(pos, dir, speed); break;
    default:
        LOG_TO_CONSOLE(L"Unknown ProjectileType in FireProjectile", L"ERROR");
        ASSERT_MSG(false, "Unknown ProjectileType in FireProjectile");
    }
    LOG_TO_CONSOLE(L"ProjectilePool: Projectile fired.", L"INFO");
}

size_t ProjectilePool::GetActiveCount() const
{
    LOG_TO_CONSOLE(L"ProjectilePool::GetActiveCount called.", L"OPERATION");
    size_t count = 0;
    for (const auto& up : m_projectiles)
        if (up->IsActive()) ++count;
    LOG_TO_CONSOLE(L"ProjectilePool: Active count=" + std::to_wstring(count), L"INFO");
    return count;
}

size_t ProjectilePool::GetAvailableCount() const
{
    LOG_TO_CONSOLE(L"ProjectilePool::GetAvailableCount called.", L"OPERATION");
    LOG_TO_CONSOLE(L"ProjectilePool: Available count=" + std::to_wstring(m_availableProjectiles.size()), L"INFO");
    return m_availableProjectiles.size();
}