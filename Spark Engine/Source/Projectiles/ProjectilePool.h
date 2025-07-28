#pragma once

#include "ProjectilePool.h"
#include "../Projectiles/Projectile.h"
#include "../Projectiles/Bullet.h"
#include "../Projectiles/Rocket.h"
#include "../Projectiles/Grenade.h"

#include <vector>
#include <queue>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

enum class ProjectileType
{
    BULLET,
    ROCKET,
    GRENADE
};

class ProjectilePool
{
public:
    explicit ProjectilePool(size_t poolSize = 200);
    ~ProjectilePool();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void    Update(float deltaTime);
    void    Render(const XMMATRIX& view, const XMMATRIX& projection);
    void    Shutdown();

    // Fire commands
    void FireBullet(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed = 100.0f);
    void FireRocket(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed = 30.0f);
    void FireGrenade(const XMFLOAT3& pos, const XMFLOAT3& dir, float speed = 15.0f);
    void FireProjectile(ProjectileType type, const XMFLOAT3& pos, const XMFLOAT3& dir, float speed);

    // Stats
    size_t GetActiveCount() const;
    size_t GetAvailableCount() const;

private:
    void CreateProjectiles();
    Projectile* GetProjectile();
    void        ReturnProjectile(Projectile* p);

    size_t                              m_poolSize;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    std::queue<Projectile*>             m_availableProjectiles;
};
