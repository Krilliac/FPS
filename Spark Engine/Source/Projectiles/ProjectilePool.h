// ProjectilePool.h
#pragma once

#include "Utils/Assert.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <queue>
#include <vector>
#include "Projectile.h"

enum class ProjectileType
{
    BULLET,
    ROCKET,
    GRENADE
};

class ProjectilePool
{
public:
    ProjectilePool(size_t poolSize);
    ~ProjectilePool();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void    Update(float deltaTime);
    void    Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
    void    Shutdown();

    Projectile* GetProjectile();
    void        ReturnProjectile(Projectile* p);

    void FireBullet(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& dir, float speed);
    void FireRocket(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& dir, float speed);
    void FireGrenade(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& dir, float speed);
    void FireProjectile(ProjectileType type, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& dir, float speed);

    size_t GetActiveCount()    const;
    size_t GetAvailableCount() const;

private:
    void CreateProjectiles();

    size_t                          m_poolSize;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    std::queue<Projectile*>         m_availableProjectiles;
};