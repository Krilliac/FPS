/**
 * @file Player.h
 * @brief Player character controller with movement, combat, and health systems
 * @author Spark Engine Team
 * @date 2025
 * 
 * The Player class represents the player character in the game world, inheriting
 * from GameObject and providing comprehensive first-person character functionality
 * including movement, jumping, weapon handling, health/armor systems, and combat.
 */

#pragma once

#include "GameObject.h"
#include "Utils/Assert.h"
#include "..\Camera\SparkEngineCamera.h"
#include "..\Input\InputManager.h"
#include "..\Projectiles\WeaponStats.h"    // Defines WeaponType and WeaponStats
#include "..\Projectiles\ProjectilePool.h"
#include "..\Utils\MathUtils.h"
#include <DirectXMath.h>

/**
 * @brief Player character controller class
 * 
 * The Player class provides a complete first-person character controller with
 * movement physics, weapon systems, health management, and combat mechanics.
 * It integrates with the camera system for first-person perspective and the
 * input system for responsive controls.
 * 
 * Features include:
 * - First-person movement (WASD, jump, crouch, run)
 * - Health and armor systems with damage/healing
 * - Weapon system with multiple weapon types and reloading
 * - Physics-based movement with gravity and collision
 * - Animation systems (head bobbing, footsteps)
 * 
 * @note The Player does not own the camera, input manager, or projectile pool references
 * @warning Ensure SetProjectilePool() is called before using weapons
 */
class Player : public GameObject
{
public:
    /**
     * @brief Default constructor
     * 
     * Initializes player stats, movement parameters, and combat variables
     * to default values suitable for first-person gameplay.
     */
    Player();

    /**
     * @brief Default destructor
     * 
     * Uses default cleanup as all resources are managed automatically.
     */
    ~Player() override = default;

    /**
     * @brief Initialize the player with required engine systems
     * 
     * Sets up the player character with DirectX resources and engine system
     * references. Configures the player for gameplay with camera and input.
     * 
     * @param device DirectX 11 device for resource creation
     * @param context DirectX 11 device context for rendering
     * @param camera Pointer to the first-person camera system
     * @param input Pointer to the input manager for controls
     * @return HRESULT indicating success or failure of initialization
     * @note The camera and input pointers are stored but not owned by Player
     */
    HRESULT Initialize(ID3D11Device* device,
        ID3D11DeviceContext* context,
        SparkEngineCamera* camera,
        InputManager* input);

    /**
     * @brief Update player logic for the current frame
     * 
     * Processes input, updates movement physics, handles combat timing,
     * updates animations, and manages player state for the current frame.
     * 
     * @param dt Delta time since last frame in seconds
     */
    void Update(float dt) override;

    /**
     * @brief Render the player (typically no visual representation in first-person)
     * 
     * In first-person mode, this typically doesn't render anything visible.
     * May be used for debugging visualization or third-person mode.
     * 
     * @param view Camera view transformation matrix
     * @param proj Camera projection transformation matrix
     */
    void Render(const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& proj) override;

    /**
     * @brief Apply damage to the player
     * 
     * Reduces player health, accounting for armor protection. Can result
     * in player death if health reaches zero.
     * 
     * @param dmg Amount of damage to apply (positive value)
     */
    void TakeDamage(float dmg);

    /**
     * @brief Restore player health
     * 
     * Increases player health up to the maximum health limit.
     * 
     * @param amt Amount of health to restore (positive value)
     */
    void Heal(float amt);

    /**
     * @brief Increase player armor
     * 
     * Adds armor points up to the maximum armor limit.
     * 
     * @param amt Amount of armor to add (positive value)
     */
    void AddArmor(float amt);

    /**
     * @brief Execute a jump if the player is grounded
     * 
     * Applies upward velocity for jumping. Only works if the player
     * is currently on the ground.
     */
    void Jump();

    /**
     * @brief Begin reloading the current weapon
     * 
     * Starts the reload process for the current weapon if not already
     * reloading and if the weapon can be reloaded.
     */
    void StartReload();

    /**
     * @brief Fire the current weapon
     * 
     * Attempts to fire the current weapon if ammunition is available,
     * the weapon is not on cooldown, and not currently reloading.
     */
    void Fire();

    /**
     * @brief Switch to a different weapon type
     * 
     * Changes the current weapon to the specified type and resets
     * weapon-specific timers and ammunition.
     * 
     * @param t New weapon type to switch to
     */
    void ChangeWeapon(WeaponType t);

    /**
     * @brief Set the projectile pool for weapon projectiles
     * 
     * Assigns the projectile pool that will be used when firing weapons.
     * This must be set before the player can use weapons.
     * 
     * @param pool Pointer to the projectile object pool
     * @warning Must not be null - assertion will fail if null is passed
     */
    void SetProjectilePool(ProjectilePool* pool)
    {
        ASSERT_NOT_NULL(pool);
        m_projectilePool = pool;
    }

    /**
     * @brief Get current player health
     * @return Current health value
     */
    float GetHealth()    const { return m_health; }

    /**
     * @brief Get maximum player health
     * @return Maximum health value
     */
    float GetMaxHealth() const { return m_maxHealth; }

    /**
     * @brief Check if the player is alive
     * @return true if health is above zero, false if dead
     */
    bool  IsAlive()      const { return m_health > 0.0f; }

    /**
     * @brief Set the running state of the player
     * @param v true to enable running, false for normal walking speed
     */
    void SetRunning(bool v) { m_isRunning = v; }

    /**
     * @brief Set the crouching state of the player
     * @param v true to crouch, false to stand normally
     */
    void SetCrouching(bool v) { m_isCrouching = v; }

    /**
     * @brief Collision callback when player hits another game object
     * 
     * Handles interactions when the player collides with other objects
     * in the game world (items, enemies, etc.).
     * 
     * @param target Pointer to the game object that was hit
     */
    void OnHit(GameObject* target) override;

    /**
     * @brief Collision callback when player hits world geometry
     * 
     * Handles collisions with static world geometry like walls, floors,
     * and other environmental objects.
     * 
     * @param hitPoint World position where the collision occurred
     * @param normal Surface normal at the collision point
     */
    void OnHitWorld(const DirectX::XMFLOAT3& hitPoint,
        const DirectX::XMFLOAT3& normal) override;

private:
    // Stats
    float m_health{ 100 }, m_maxHealth{ 100 };     ///< Current and maximum health
    float m_armor{ 0 }, m_maxArmor{ 100 };         ///< Current and maximum armor
    float m_stamina{ 100 }, m_maxStamina{ 100 };   ///< Current and maximum stamina
    float m_speed{ 5 }, m_jumpHeight{ 3 };         ///< Movement speed and jump height

    // Movement
    DirectX::XMFLOAT3 m_velocity{};                ///< Current velocity vector
    bool m_isGrounded{ true }, m_isRunning{ false }, ///< Movement state flags
        m_isCrouching{ false }, m_isJumping{ false };

    // Combat
    WeaponStats m_currentWeapon;    ///< Current weapon configuration
    int         m_currentAmmo{ 0 }; ///< Current ammunition count
    float       m_fireTimer{ 0 }, m_reloadTimer{ 0 }; ///< Weapon timing
    bool        m_isReloading{ false }, m_isFiring{ false }; ///< Combat state

    // External references (not owned)
    SparkEngineCamera* m_camera{ nullptr };      ///< Reference to camera system
    InputManager* m_input{ nullptr };            ///< Reference to input manager
    ProjectilePool* m_projectilePool{ nullptr }; ///< Reference to projectile pool

    // Collision & animation
    BoundingSphere m_collisionSphere;            ///< Collision bounds for player
    float          m_bobTimer{ 0 }, m_footstepTimer{ 0 }; ///< Animation timers

    /**
     * @brief Process input for movement and actions
     * @param dt Delta time for frame-rate independent input
     */
    void HandleInput(float dt);

    /**
     * @brief Update movement based on input and physics
     * @param dt Delta time for frame-rate independent movement
     */
    void UpdateMovement(float dt);

    /**
     * @brief Update weapon timers and combat state
     * @param dt Delta time for frame-rate independent timing
     */
    void UpdateCombat(float dt);

    /**
     * @brief Update physics simulation (gravity, velocity, etc.)
     * @param dt Delta time for frame-rate independent physics
     */
    void UpdatePhysics(float dt);

    /**
     * @brief Update animation timers and effects
     * @param dt Delta time for frame-rate independent animation
     */
    void UpdateAnimation(float dt);

    /**
     * @brief Check and resolve collisions with world geometry
     */
    void UpdateCollision();

    /**
     * @brief Apply gravity to player velocity
     * @param dt Delta time for frame-rate independent gravity
     */
    void ApplyGravity(float dt);

    /**
     * @brief Check if player is touching the ground
     */
    void CheckGroundCollision();

    /**
     * @brief Handle footstep sound timing
     * @param dt Delta time for frame-rate independent audio
     */
    void HandleFootsteps(float dt);

    /**
     * @brief Get weapon statistics for a specific weapon type
     * @param type Weapon type to get stats for
     * @return WeaponStats structure with weapon parameters
     */
    WeaponStats       GetWeaponStats(WeaponType type);

    /**
     * @brief Calculate the direction vector for projectile firing
     * @return Normalized direction vector from camera forward
     */
    DirectX::XMFLOAT3 CalculateFireDirection();
};