#include "Player.h"
#include "..\Camera\FPSCamera.h"
#include "..\Input\InputManager.h"
#include "..\Projectiles\ProjectilePool.h"
#include "..\Utils\MathUtils.h"

Player::Player()
    : GameObject()
    , m_health(100.0f)
    , m_maxHealth(100.0f)
    , m_armor(0.0f)
    , m_maxArmor(100.0f)
    , m_stamina(100.0f)
    , m_maxStamina(100.0f)
    , m_speed(5.0f)
    , m_jumpHeight(3.0f)
    , m_velocity(0.0f, 0.0f, 0.0f)
    , m_isGrounded(true)
    , m_isRunning(false)
    , m_isCrouching(false)
    , m_isJumping(false)
    , m_currentAmmo(30)
    , m_fireTimer(0.0f)
    , m_reloadTimer(0.0f)
    , m_isReloading(false)
    , m_isFiring(false)
    , m_camera(nullptr)
    , m_input(nullptr)
    , m_projectilePool(nullptr)
    , m_bobTimer(0.0f)
    , m_footstepTimer(0.0f)
{
    SetName("Player");
    m_currentWeapon = GetWeaponStats(WeaponType::PISTOL);
    m_collisionSphere = BoundingSphere(m_position, 0.5f);
}

Player::~Player()
{
}

HRESULT Player::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, 
                          FPSCamera* camera, InputManager* input)
{
    m_camera = camera;
    m_input = input;
    
    // Player doesn't need a visible mesh in first person
    SetVisible(false);
    
    return GameObject::Initialize(device, context);
}

void Player::Update(float deltaTime)
{
    if (!IsAlive()) return;
    
    HandleInput(deltaTime);
    UpdateMovement(deltaTime);
    UpdateCombat(deltaTime);
    UpdatePhysics(deltaTime);
    UpdateAnimation(deltaTime);
    UpdateCollision();
    
    GameObject::Update(deltaTime);
}

void Player::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    // Player is typically not rendered in first-person view
    // Override if third-person view is needed
}

void Player::TakeDamage(float damage)
{
    if (!IsAlive()) return;
    
    // Apply armor absorption
    float damageToArmor = std::min(damage * 0.5f, m_armor);
    m_armor -= damageToArmor;
    damage -= damageToArmor;
    
    // Apply remaining damage to health
    m_health -= damage;
    m_health = std::max(0.0f, m_health);
    
    // TODO: Play damage sound/effect
}

void Player::Heal(float amount)
{
    m_health = std::min(m_maxHealth, m_health + amount);
}

void Player::AddArmor(float amount)
{
    m_armor = std::min(m_maxArmor, m_armor + amount);
}

void Player::Jump()
{
    if (m_isGrounded && !m_isJumping && m_stamina > 20.0f)
    {
        m_velocity.y = m_jumpHeight;
        m_isJumping = true;
        m_isGrounded = false;
        m_stamina -= 20.0f;
        
        // TODO: Play jump sound
    }
}

void Player::StartReload()
{
    if (!m_isReloading && m_currentAmmo < m_currentWeapon.MagazineSize)
    {
        m_isReloading = true;
        m_reloadTimer = m_currentWeapon.ReloadTime;
        
        // TODO: Play reload sound
    }
}

void Player::Fire()
{
    if (m_isReloading || m_currentAmmo <= 0 || m_fireTimer > 0.0f) return;
    
    if (m_projectilePool && m_camera)
    {
        XMFLOAT3 firePosition = m_camera->GetPosition();
        XMFLOAT3 fireDirection = CalculateFireDirection();
        
        // Fire different projectile types based on weapon
        switch (m_currentWeapon.Type)
        {
        case WeaponType::PISTOL:
        case WeaponType::RIFLE:
            m_projectilePool->FireBullet(firePosition, fireDirection, 100.0f);
            break;
        case WeaponType::ROCKET_LAUNCHER:
            m_projectilePool->FireRocket(firePosition, fireDirection, 30.0f);
            break;
        case WeaponType::GRENADE_LAUNCHER:
            m_projectilePool->FireGrenade(firePosition, fireDirection, 15.0f);
            break;
        }
        
        m_currentAmmo--;
        m_fireTimer = 1.0f / m_currentWeapon.FireRate;
        m_isFiring = true;
        
        // TODO: Play fire sound
    }
}

void Player::ChangeWeapon(WeaponType weaponType)
{
    m_currentWeapon = GetWeaponStats(weaponType);
    m_currentAmmo = m_currentWeapon.MagazineSize;
    m_isReloading = false;
    m_fireTimer = 0.0f;
}

void Player::HandleInput(float deltaTime)
{
    if (!m_input) return;
    
    // Weapon controls
    if (m_input->WasKeyPressed('R'))
    {
        StartReload();
    }
    
    if (m_input->IsMouseButtonDown(0))
    {
        Fire();
    }
    
    // Movement controls
    if (m_input->WasKeyPressed(VK_SPACE))
    {
        Jump();
    }
    
    m_isRunning = m_input->IsKeyDown(VK_LSHIFT);
    m_isCrouching = m_input->IsKeyDown(VK_LCONTROL);
    
    // Weapon switching
    if (m_input->WasKeyPressed('1'))
        ChangeWeapon(WeaponType::PISTOL);
    else if (m_input->WasKeyPressed('2'))
        ChangeWeapon(WeaponType::RIFLE);
    else if (m_input->WasKeyPressed('3'))
        ChangeWeapon(WeaponType::SHOTGUN);
    else if (m_input->WasKeyPressed('4'))
        ChangeWeapon(WeaponType::ROCKET_LAUNCHER);
    else if (m_input->WasKeyPressed('5'))
        ChangeWeapon(WeaponType::GRENADE_LAUNCHER);
}

void Player::UpdateMovement(float deltaTime)
{
    if (!m_input || !m_camera) return;
    
    XMFLOAT3 movement(0.0f, 0.0f, 0.0f);
    float currentSpeed = m_speed;
    
    // Modify speed based on state
    if (m_isRunning && m_stamina > 0.0f)
    {
        currentSpeed *= 2.0f;
        m_stamina -= 30.0f * deltaTime;
    }
    else if (m_isCrouching)
    {
        currentSpeed *= 0.5f;
    }
    
    // Get movement input
    if (m_input->IsKeyDown('W'))
        movement.z += 1.0f;
    if (m_input->IsKeyDown('S'))
        movement.z -= 1.0f;
    if (m_input->IsKeyDown('A'))
        movement.x -= 1.0f;
    if (m_input->IsKeyDown('D'))
        movement.x += 1.0f;
    
    // Normalize movement vector
    XMVECTOR moveVec = XMLoadFloat3(&movement);
    if (XMVectorGetX(XMVector3Length(moveVec)) > 0.0f)
    {
        moveVec = XMVector3Normalize(moveVec);
        XMStoreFloat3(&movement, moveVec);
        
        // Apply movement relative to camera direction
        XMFLOAT3 forward = m_camera->GetForward();
        XMFLOAT3 right = XMFLOAT3(1.0f, 0.0f, 0.0f); // Simplified right vector
        
        XMFLOAT3 finalMovement;
        finalMovement.x = (right.x * movement.x + forward.x * movement.z) * currentSpeed * deltaTime;
        finalMovement.y = 0.0f; // Don't move vertically with WASD
        finalMovement.z = (right.z * movement.x + forward.z * movement.z) * currentSpeed * deltaTime;
        
        Translate(finalMovement);
        
        // Update footstep timer
        HandleFootsteps(deltaTime);
    }
    
    // Regenerate stamina when not running
    if (!m_isRunning)
    {
        m_stamina = std::min(m_maxStamina, m_stamina + 50.0f * deltaTime);
    }
}

void Player::UpdateCombat(float deltaTime)
{
    // Update fire timer
    if (m_fireTimer > 0.0f)
    {
        m_fireTimer -= deltaTime;
        if (m_fireTimer <= 0.0f)
        {
            m_isFiring = false;
        }
    }
    
    // Update reload timer
    if (m_isReloading)
    {
        m_reloadTimer -= deltaTime;
        if (m_reloadTimer <= 0.0f)
        {
            m_currentAmmo = m_currentWeapon.MagazineSize;
            m_isReloading = false;
            
            // TODO: Play reload complete sound
        }
    }
}

void Player::UpdatePhysics(float deltaTime)
{
    ApplyGravity(deltaTime);
    CheckGroundCollision();
    
    // Update position with velocity
    XMFLOAT3 velocityMovement;
    velocityMovement.x = m_velocity.x * deltaTime;
    velocityMovement.y = m_velocity.y * deltaTime;
    velocityMovement.z = m_velocity.z * deltaTime;
    
    Translate(velocityMovement);
    
    // Update camera position to match player
    if (m_camera)
    {
        XMFLOAT3 cameraOffset(0.0f, 1.7f, 0.0f); // Eye height
        if (m_isCrouching)
            cameraOffset.y *= 0.6f; // Lower when crouching
            
        XMFLOAT3 cameraPos;
        cameraPos.x = m_position.x + cameraOffset.x;
        cameraPos.y = m_position.y + cameraOffset.y;
        cameraPos.z = m_position.z + cameraOffset.z;
        
        m_camera->SetPosition(cameraPos);
    }
}

void Player::UpdateAnimation(float deltaTime)
{
    // Simple head bobbing when moving
    if (m_input && (m_input->IsKeyDown('W') || m_input->IsKeyDown('S') || 
                   m_input->IsKeyDown('A') || m_input->IsKeyDown('D')))
    {
        m_bobTimer += deltaTime * 8.0f;
        if (m_camera)
        {
            float bobAmount = sinf(m_bobTimer) * 0.05f;
            // Apply subtle head bob effect to camera
        }
    }
    else
    {
        m_bobTimer = 0.0f;
    }
}

void Player::UpdateCollision()
{
    m_collisionSphere.Center = m_position;
}

void Player::ApplyGravity(float deltaTime)
{
    if (!m_isGrounded)
    {
        const float gravity = -9.8f;
        m_velocity.y += gravity * deltaTime;
    }
}

void Player::CheckGroundCollision()
{
    // Simple ground check
    if (m_position.y <= 0.0f)
    {
        m_position.y = 0.0f;
        m_velocity.y = 0.0f;
        m_isGrounded = true;
        m_isJumping = false;
        SetPosition(m_position);
    }
    else
    {
        m_isGrounded = false;
    }
}

void Player::HandleFootsteps(float deltaTime)
{
    m_footstepTimer -= deltaTime;
    if (m_footstepTimer <= 0.0f)
    {
        // TODO: Play footstep sound
        m_footstepTimer = m_isRunning ? 0.3f : 0.6f; // Faster steps when running
    }
}

WeaponStats Player::GetWeaponStats(WeaponType type)
{
    WeaponStats stats;
    stats.Type = type;
    
    switch (type)
    {
    case WeaponType::PISTOL:
        stats.Damage = 20.0f;
        stats.FireRate = 3.0f;
        stats.MagazineSize = 12;
        stats.ReloadTime = 1.5f;
        stats.Range = 50.0f;
        stats.Accuracy = 0.9f;
        break;
        
    case WeaponType::RIFLE:
        stats.Damage = 35.0f;
        stats.FireRate = 8.0f;
        stats.MagazineSize = 30;
        stats.ReloadTime = 2.0f;
        stats.Range = 100.0f;
        stats.Accuracy = 0.95f;
        break;
        
    case WeaponType::SHOTGUN:
        stats.Damage = 60.0f;
        stats.FireRate = 1.5f;
        stats.MagazineSize = 8;
        stats.ReloadTime = 3.0f;
        stats.Range = 25.0f;
        stats.Accuracy = 0.7f;
        break;
        
    case WeaponType::ROCKET_LAUNCHER:
        stats.Damage = 150.0f;
        stats.FireRate = 0.5f;
        stats.MagazineSize = 1;
        stats.ReloadTime = 4.0f;
        stats.Range = 200.0f;
        stats.Accuracy = 0.95f;
        break;
        
    case WeaponType::GRENADE_LAUNCHER:
        stats.Damage = 100.0f;
        stats.FireRate = 1.0f;
        stats.MagazineSize = 6;
        stats.ReloadTime = 3.5f;
        stats.Range = 80.0f;
        stats.Accuracy = 0.8f;
        break;
    }
    
    return stats;
}

XMFLOAT3 Player::CalculateFireDirection()
{
    if (!m_camera) return XMFLOAT3(0.0f, 0.0f, 1.0f);
    
    XMFLOAT3 direction = m_camera->GetForward();
    
    // Apply weapon accuracy (add random spread)
    float inaccuracy = 1.0f - m_currentWeapon.Accuracy;
    if (inaccuracy > 0.0f)
    {
        float spreadAngle = inaccuracy * 0.1f; // Convert to radians
        
        // Add random spread
        float randomX = MathUtils::RandomFloat(-spreadAngle, spreadAngle);
        float randomY = MathUtils::RandomFloat(-spreadAngle, spreadAngle);
        
        // Apply spread to direction
        XMVECTOR dirVec = XMLoadFloat3(&direction);
        XMMATRIX spreadMatrix = XMMatrixRotationRollPitchYaw(randomY, randomX, 0.0f);
        dirVec = XMVector3TransformCoord(dirVec, spreadMatrix);
        XMStoreFloat3(&direction, dirVec);
    }
    
    return direction;
}
