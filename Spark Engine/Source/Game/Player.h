#pragma once

#include "GameObject.h"
#include "Utils/Assert.h"
#include "..\Camera\SparkEngineCamera.h"
#include "..\Input\InputManager.h"
#include "..\Projectiles\WeaponStats.h"    // Defines WeaponType and WeaponStats
#include "..\Projectiles\ProjectilePool.h"
#include "..\Utils\MathUtils.h"
#include <DirectXMath.h>

class Player : public GameObject
{
public:
    Player();
    ~Player() override = default;

    // Note: signature matches GameObject::Initialize
    HRESULT Initialize(ID3D11Device* device,
        ID3D11DeviceContext* context,
        SparkEngineCamera* camera,
        InputManager* input);

    void Update(float dt) override;
    void Render(const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& proj) override;

    // Damage & healing
    void TakeDamage(float dmg);
    void Heal(float amt);
    void AddArmor(float amt);

    // Movement & combat
    void Jump();
    void StartReload();
    void Fire();
    void ChangeWeapon(WeaponType t);

    // Assign external pool
    void SetProjectilePool(ProjectilePool* pool)
    {
        ASSERT_NOT_NULL(pool);
        m_projectilePool = pool;
    }

    // Status
    float GetHealth()    const { return m_health; }
    float GetMaxHealth() const { return m_maxHealth; }
    bool  IsAlive()      const { return m_health > 0.0f; }

    void SetRunning(bool v) { m_isRunning = v; }
    void SetCrouching(bool v) { m_isCrouching = v; }

    // Hit callbacks
    void OnHit(GameObject* target) override;
    void OnHitWorld(const DirectX::XMFLOAT3& hitPoint,
        const DirectX::XMFLOAT3& normal) override;

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
    int         m_currentAmmo{ 0 };
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

    WeaponStats       GetWeaponStats(WeaponType type);
    DirectX::XMFLOAT3 CalculateFireDirection();
};