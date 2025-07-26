#pragma once
#include "GameObject.h"
#include "..\Physics\CollisionSystem.h"

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
    float Damage;
    float FireRate;          // Rounds per second
    int MagazineSize;
    float ReloadTime;
    float Range;
    float Accuracy;
    WeaponType Type;
    
    WeaponStats() 
        : Damage(25.0f), FireRate(5.0f), MagazineSize(30), ReloadTime(2.0f)
        , Range(100.0f), Accuracy(0.95f), Type(WeaponType::PISTOL) {}
};

class Player : public GameObject
{
private:
    // Player stats
    float m_health;
    float m_maxHealth;
    float m_armor;
    float m_maxArmor;
    float m_stamina;
    float m_maxStamina;
    float m_speed;
    float m_jumpHeight;
    
    // Movement
    XMFLOAT3 m_velocity;
    bool m_isGrounded;
    bool m_isRunning;
    bool m_isCrouching;
    bool m_isJumping;
    
    // Combat
    WeaponStats m_currentWeapon;
    int m_currentAmmo;
    float m_fireTimer;
    float m_reloadTimer;
    bool m_isReloading;
    bool m_isFiring;
    
    // Input and camera
    FPSCamera* m_camera;
    InputManager* m_input;
    ProjectilePool* m_projectilePool;
    
    // Physics
    BoundingSphere m_collisionSphere;
    
    // Animation/State
    float m_bobTimer;
    float m_footstepTimer;
    
public:
    Player();
    ~Player();

    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context, 
                      FPSCamera* camera, InputManager* input);
    void Update(float deltaTime) override;
    void Render(const XMMATRIX& view, const XMMATRIX& projection) override;

    // Player actions
    void TakeDamage(float damage);
    void Heal(float amount);
    void AddArmor(float amount);
    void Jump();
    void StartReload();
    void Fire();
    
    // Movement
    void SetRunning(bool running) { m_isRunning = running; }
    void SetCrouching(bool crouching) { m_isCrouching = crouching; }
    
    // Weapon management
    void ChangeWeapon(WeaponType weaponType);
    void SetProjectilePool(ProjectilePool* pool) { m_projectilePool = pool; }
    
    // Accessors
    float GetHealth() const { return m_health; }
    float GetMaxHealth() const { return m_maxHealth; }
    float GetHealthPercentage() const { return m_health / m_maxHealth; }
    
    float GetArmor() const { return m_armor; }
    float GetMaxArmor() const { return m_maxArmor; }
    float GetArmorPercentage() const { return m_armor / m_maxArmor; }
    
    float GetStamina() const { return m_stamina; }
    float GetMaxStamina() const { return m_maxStamina; }
    float GetStaminaPercentage() const { return m_stamina / m_maxStamina; }
    
    int GetCurrentAmmo() const { return m_currentAmmo; }
    int GetMagazineSize() const { return m_currentWeapon.MagazineSize; }
    bool IsReloading() const { return m_isReloading; }
    
    const WeaponStats& GetCurrentWeapon() const { return m_currentWeapon; }
    const BoundingSphere& GetCollisionSphere() const { return m_collisionSphere; }
    
    bool IsAlive() const { return m_health > 0.0f; }
    bool IsGrounded() const { return m_isGrounded; }

private:
    void HandleInput(float deltaTime);
    void UpdateMovement(float deltaTime);
    void UpdateCombat(float deltaTime);
    void UpdatePhysics(float deltaTime);
    void UpdateAnimation(float deltaTime);
    void UpdateCollision();
    
    void ApplyGravity(float deltaTime);
    void CheckGroundCollision();
    void HandleFootsteps(float deltaTime);
    
    WeaponStats GetWeaponStats(WeaponType type);
    XMFLOAT3 CalculateFireDirection();
};
