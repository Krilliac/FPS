#pragma once

#include "..\Core\framework.h"
#include "GameObject.h"
#include "..\Camera\SparkEngineCamera.h"
#include "..\Input\InputManager.h"
#include "..\Projectiles\ProjectilePool.h"
#include "..\Utils\MathUtils.h"
#include <algorithm>
#include <DirectXMath.h>

using DirectX::XMFLOAT3;

// Weapon definitions
enum class WeaponType
{
    PISTOL,
    RIFLE,
    SHOTGUN,
    ROCKET_LAUNCHER,
    GRENADE_LAUNCHER
};

struct WeaponStats
{
    float      Damage{};
    float      FireRate{};      // rounds/sec
    int        MagazineSize{};
    float      ReloadTime{};
    float      Range{};
    float      Accuracy{};      // 0–1
    WeaponType Type{};
    WeaponStats() = default;
    WeaponStats(float dmg, float rate, int mag,
        float reload, float range, float acc,
        WeaponType t)
        : Damage(dmg), FireRate(rate),
        MagazineSize(mag), ReloadTime(reload),
        Range(range), Accuracy(acc), Type(t)
    {
    }
};

class BoundingSphere;

class Player : public GameObject
{
public:
    Player();
    ~Player() override = default;

    HRESULT Initialize(ID3D11Device* device,
        ID3D11DeviceContext* context,
        SparkEngineCamera* camera,
        InputManager* input);

    void Update(float dt) override;
    void Render(const DirectX::XMMATRIX&,
        const DirectX::XMMATRIX&) override;

    void OnHit(GameObject*) override {}
    void OnHitWorld(const XMFLOAT3&, const XMFLOAT3&) override {}

    void TakeDamage(float dmg);
    void Heal(float amt);
    void AddArmor(float amt);
    void Jump();
    void StartReload();
    void Fire();

    void SetRunning(bool v) { m_isRunning = v; }
    void SetCrouching(bool v) { m_isCrouching = v; }
    void ChangeWeapon(WeaponType t);
    void SetProjectilePool(ProjectilePool* pool)
    {
        m_projectilePool = pool;
    }

    float GetHealth()    const { return m_health; }
    float GetMaxHealth() const { return m_maxHealth; }
    bool  IsAlive()      const { return m_health > 0.0f; }

private:
    // Stats
    float m_health{ 100 }, m_maxHealth{ 100 };
    float m_armor{ 0 }, m_maxArmor{ 100 };
    float m_stamina{ 100 }, m_maxStamina{ 100 };
    float m_speed{ 5 }, m_jumpHeight{ 3 };

    // Movement
    DirectX::XMFLOAT3 m_velocity{};
    bool m_isGrounded{ true }, m_isRunning{ false },
        m_isCrouching{ false }, m_isJumping{ false };

    // Combat
    WeaponStats m_currentWeapon;
    int         m_currentAmmo{ 30 };
    float       m_fireTimer{ 0 }, m_reloadTimer{ 0 };
    bool        m_isReloading{ false }, m_isFiring{ false };

    // External
    SparkEngineCamera* m_camera{ nullptr };
    InputManager* m_input{ nullptr };
    ProjectilePool* m_projectilePool{ nullptr };

    // Collision & animation
    BoundingSphere m_collisionSphere;
    float          m_bobTimer{ 0 }, m_footstepTimer{ 0 };

    // Helpers
    void HandleInput(float dt);
    void UpdateMovement(float dt);
    void UpdateCombat(float dt);
    void UpdatePhysics(float dt);
    void UpdateAnimation(float dt);
    void UpdateCollision();
    void ApplyGravity(float dt);
    void CheckGroundCollision();
    void HandleFootsteps(float dt);

    WeaponStats GetWeaponStats(WeaponType type);
    DirectX::XMFLOAT3 CalculateFireDirection();
};
