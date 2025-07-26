#pragma once

#include "GameObject.h"
#include "..\Physics\CollisionSystem.h"
#include <DirectXMath.h>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

class FPSCamera;
class InputManager;
class ProjectilePool;

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
    float      Damage;
    float      FireRate;      // rounds/sec
    int        MagazineSize;
    float      ReloadTime;
    float      Range;
    float      Accuracy;      // 0–1
    WeaponType Type;

    WeaponStats() = default;
    WeaponStats(float dmg, float rate, int mag, float reload, float range, float acc, WeaponType t)
        : Damage(dmg)
        , FireRate(rate)
        , MagazineSize(mag)
        , ReloadTime(reload)
        , Range(range)
        , Accuracy(acc)
        , Type(t)
    {
    }
};

class Player : public GameObject
{
public:
    Player();
    ~Player() override = default;

    // Not override: signature differs
    HRESULT Initialize(ID3D11Device* device,
        ID3D11DeviceContext* context,
        FPSCamera* camera,
        InputManager* input);

    void Update(float deltaTime) override;
    void Render(const XMMATRIX& view, const XMMATRIX& proj) override;

    // Implement callbacks so Player is no longer abstract
    void OnHit(GameObject* target) override {}
    void OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal) override {}

    // Actions
    void TakeDamage(float dmg);
    void Heal(float amt);
    void AddArmor(float amt);
    void Jump();
    void StartReload();
    void Fire();

    // Movement state
    void SetRunning(bool v) { m_isRunning = v; }
    void SetCrouching(bool v) { m_isCrouching = v; }

    // Weapons
    void ChangeWeapon(WeaponType type);
    void SetProjectilePool(ProjectilePool* pool) { m_projectilePool = pool; }

    // Accessors
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
    XMFLOAT3 m_velocity{};
    bool     m_isGrounded{ true }, m_isRunning{ false },
        m_isCrouching{ false }, m_isJumping{ false };

    // Combat
    WeaponStats     m_currentWeapon;
    int             m_currentAmmo{ 30 };
    float           m_fireTimer{ 0 }, m_reloadTimer{ 0 };
    bool            m_isReloading{ false }, m_isFiring{ false };

    // External
    FPSCamera* m_camera{ nullptr };
    InputManager* m_input{ nullptr };
    ProjectilePool* m_projectilePool{ nullptr };

    // Collision & animation
    BoundingSphere  m_collisionSphere;
    float           m_bobTimer{ 0 }, m_footstepTimer{ 0 };

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
    XMFLOAT3    CalculateFireDirection();
};