#pragma once
#include "Projectile.h"
#include <vector>
#include <queue>

enum class ProjectileType
{
    BULLET,
    ROCKET,
    GRENADE
};

class ProjectilePool
{
private:
    std::vector<std::unique_ptr<Projectile>> m_projectiles;
    std::queue<Projectile*> m_availableProjectiles;
    
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
    
    size_t m_poolSize;

public:
    ProjectilePool(size_t poolSize = 200);
    ~ProjectilePool();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Update(float deltaTime);
    void Render(const XMMATRIX& view, const XMMATRIX& projection);
    void Shutdown();

    // Pool management
    Projectile* GetProjectile();
    void ReturnProjectile(Projectile* projectile);
    
    // Projectile creation
    void FireProjectile(ProjectileType type, const XMFLOAT3& startPosition, 
                       const XMFLOAT3& direction, float speed = 50.0f);
    void FireBullet(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed = 100.0f);
    void FireRocket(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed = 30.0f);
    void FireGrenade(const XMFLOAT3& startPosition, const XMFLOAT3& direction, float speed = 15.0f);
    
    // Utility
    size_t GetActiveCount() const;
    size_t GetAvailableCount() const { return m_availableProjectiles.size(); }
    void ClearAll();

private:
    void CreateProjectiles();
    Projectile* CreateProjectileOfType(ProjectileType type);
};
