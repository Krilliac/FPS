#include "Player.h"
#include "..\Camera\FPSCamera.h"
#include "..\Input\InputManager.h"
#include "..\Projectiles\ProjectilePool.h"
#include "..\Utils\MathUtils.h"
#include <algorithm>

Player::Player()
    : m_health(100.0f)
    , m_maxHealth(100.0f)
    , m_armor(0.0f)
    , m_maxArmor(100.0f)
    , m_stamina(100.0f)
    , m_maxStamina(100.0f)
    , m_speed(5.0f)
    , m_jumpHeight(3.0f)
    , m_velocity(0, 0, 0)
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
    m_collisionSphere = BoundingSphere(GetPosition(), 0.5f);
}

Player::~Player() = default;

HRESULT Player::Initialize(ID3D11Device* device,
    ID3D11DeviceContext* context,
    FPSCamera* camera,
    InputManager* input)
{
    m_camera = camera;
    m_input = input;
    SetVisible(false);  // first-person hide

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

void Player::Render(const XMMATRIX& view,
    const XMMATRIX& projection)
{
    // No first-person mesh
}

void Player::TakeDamage(float damage)
{
    if (!IsAlive()) return;
    float armorHit = (std::min)(damage * 0.5f, m_armor);
    m_armor -= armorHit;
    m_health = (std::max)(0.0f, m_health - (damage - armorHit));
}

void Player::Heal(float amount)
{
    m_health = (std::min)(m_maxHealth, m_health + amount);
}

void Player::AddArmor(float amount)
{
    m_armor = (std::min)(m_maxArmor, m_armor + amount);
}

void Player::Jump()
{
    if (m_isGrounded && !m_isJumping && m_stamina > 20.0f)
    {
        m_velocity.y = m_jumpHeight;
        m_isJumping = true;
        m_isGrounded = false;
        m_stamina -= 20.0f;
    }
}

void Player::StartReload()
{
    if (!m_isReloading && m_currentAmmo < m_currentWeapon.MagazineSize)
    {
        m_isReloading = true;
        m_reloadTimer = m_currentWeapon.ReloadTime;
    }
}

void Player::Fire()
{
    if (m_isReloading || m_currentAmmo <= 0 || m_fireTimer > 0.0f)
        return;

    if (m_projectilePool && m_camera)
    {
        auto pos = m_camera->GetPosition();
        auto dir = CalculateFireDirection();

        switch (m_currentWeapon.Type)
        {
        case WeaponType::PISTOL:
        case WeaponType::RIFLE:
            m_projectilePool->FireBullet(pos, dir, 100.0f);
            break;
        case WeaponType::ROCKET_LAUNCHER:
            m_projectilePool->FireRocket(pos, dir, 30.0f);
            break;
        case WeaponType::GRENADE_LAUNCHER:
            m_projectilePool->FireGrenade(pos, dir, 15.0f);
            break;
        }

        --m_currentAmmo;
        m_fireTimer = 1.0f / m_currentWeapon.FireRate;
        m_isFiring = true;
    }
}

void Player::ChangeWeapon(WeaponType type)
{
    m_currentWeapon = GetWeaponStats(type);
    m_currentAmmo = m_currentWeapon.MagazineSize;
    m_isReloading = false;
    m_fireTimer = 0.0f;
}

void Player::HandleInput(float deltaTime)
{
    if (!m_input) return;

    if (m_input->WasKeyPressed('R')) StartReload();
    if (m_input->IsMouseButtonDown(0)) Fire();
    if (m_input->WasKeyPressed(VK_SPACE)) Jump();

    m_isRunning = m_input->IsKeyDown(VK_LSHIFT);
    m_isCrouching = m_input->IsKeyDown(VK_LCONTROL);

    if (m_input->WasKeyPressed('1')) ChangeWeapon(WeaponType::PISTOL);
    if (m_input->WasKeyPressed('2')) ChangeWeapon(WeaponType::RIFLE);
    if (m_input->WasKeyPressed('3')) ChangeWeapon(WeaponType::SHOTGUN);
    if (m_input->WasKeyPressed('4')) ChangeWeapon(WeaponType::ROCKET_LAUNCHER);
    if (m_input->WasKeyPressed('5')) ChangeWeapon(WeaponType::GRENADE_LAUNCHER);
}

void Player::UpdateMovement(float deltaTime)
{
    if (!m_input || !m_camera) return;

    XMFLOAT3 move{ 0,0,0 };
    float speed = m_speed;
    if (m_isRunning && m_stamina > 0)
    {
        speed *= 2.0f;
        m_stamina -= 30.0f * deltaTime;
    }
    else if (m_isCrouching)
    {
        speed *= 0.5f;
    }

    if (m_input->IsKeyDown('W')) move.z += 1;
    if (m_input->IsKeyDown('S')) move.z -= 1;
    if (m_input->IsKeyDown('A')) move.x -= 1;
    if (m_input->IsKeyDown('D')) move.x += 1;

    XMVECTOR v = XMLoadFloat3(&move);
    if (XMVectorGetX(XMVector3Length(v)) > 0)
    {
        v = XMVector3Normalize(v);
        XMStoreFloat3(&move, v);

        auto fw = m_camera->GetForward();
        XMFLOAT3 rt{ 1,0,0 };
        XMFLOAT3 delta{
            (rt.x * move.x + fw.x * move.z) * speed * deltaTime,
            0,
            (rt.z * move.x + fw.z * move.z) * speed * deltaTime
        };
        Translate(delta);
        HandleFootsteps(deltaTime);
    }

    if (!m_isRunning)
        m_stamina = (std::min)(m_maxStamina, m_stamina + 50.0f * deltaTime);
}

void Player::UpdateCombat(float deltaTime)
{
    if (m_fireTimer > 0)
    {
        m_fireTimer -= deltaTime;
        if (m_fireTimer <= 0) m_isFiring = false;
    }
    if (m_isReloading)
    {
        m_reloadTimer -= deltaTime;
        if (m_reloadTimer <= 0)
        {
            m_currentAmmo = m_currentWeapon.MagazineSize;
            m_isReloading = false;
        }
    }
}

void Player::UpdatePhysics(float deltaTime)
{
    ApplyGravity(deltaTime);
    CheckGroundCollision();

    XMFLOAT3 dm{
        m_velocity.x * deltaTime,
        m_velocity.y * deltaTime,
        m_velocity.z * deltaTime
    };
    Translate(dm);

    if (m_camera)
    {
        XMFLOAT3 eye{ 0,1.7f,0 };
        if (m_isCrouching) eye.y *= 0.6f;
        XMFLOAT3 camPos{
            m_position.x + eye.x,
            m_position.y + eye.y,
            m_position.z + eye.z
        };
        m_camera->SetPosition(camPos);
    }
}

void Player::UpdateAnimation(float deltaTime)
{
    bool moving = m_input &&
        (m_input->IsKeyDown('W') || m_input->IsKeyDown('A') ||
            m_input->IsKeyDown('S') || m_input->IsKeyDown('D'));
    if (moving) m_bobTimer += deltaTime * 8.0f;
    else        m_bobTimer = 0.0f;
}

void Player::UpdateCollision()
{
    m_collisionSphere.Center = m_position;
}

void Player::ApplyGravity(float deltaTime)
{
    if (!m_isGrounded)
        m_velocity.y += -9.8f * deltaTime;
}

void Player::CheckGroundCollision()
{
    if (m_position.y <= 0)
    {
        m_position.y = 0;
        m_velocity.y = 0;
        m_isGrounded = true;
        m_isJumping = false;
        SetPosition(m_position);
    }
    else m_isGrounded = false;
}

void Player::HandleFootsteps(float deltaTime)
{
    m_footstepTimer -= deltaTime;
    if (m_footstepTimer <= 0)
    {
        // play step
        m_footstepTimer = m_isRunning ? 0.3f : 0.6f;
    }
}

WeaponStats Player::GetWeaponStats(WeaponType type)
{
    WeaponStats s; s.Type = type;
    switch (type)
    {
    case WeaponType::PISTOL:          s.Damage = 20; s.FireRate = 3; s.MagazineSize = 12; s.ReloadTime = 1.5f; s.Range = 50;  s.Accuracy = 0.9f;  break;
    case WeaponType::RIFLE:           s.Damage = 35; s.FireRate = 8; s.MagazineSize = 30; s.ReloadTime = 2.0f; s.Range = 100; s.Accuracy = 0.95f; break;
    case WeaponType::SHOTGUN:         s.Damage = 60; s.FireRate = 1.5f; s.MagazineSize = 8; s.ReloadTime = 3.0f; s.Range = 25;  s.Accuracy = 0.7f;  break;
    case WeaponType::ROCKET_LAUNCHER: s.Damage = 150; s.FireRate = 0.5f; s.MagazineSize = 1; s.ReloadTime = 4.0f; s.Range = 200; s.Accuracy = 0.95f; break;
    case WeaponType::GRENADE_LAUNCHER: s.Damage = 100; s.FireRate = 1.0f; s.MagazineSize = 6; s.ReloadTime = 3.5f; s.Range = 80;  s.Accuracy = 0.8f;  break;
    }
    return s;
}

XMFLOAT3 Player::CalculateFireDirection()
{
    XMFLOAT3 dir = m_camera ? m_camera->GetForward() : XMFLOAT3{ 0,0,1 };
    float spread = 1.0f - m_currentWeapon.Accuracy;
    if (spread > 0)
    {
        float angle = spread * 0.1f;
        float rx = MathUtils::RandomFloat(-angle, angle);
        float ry = MathUtils::RandomFloat(-angle, angle);
        XMVECTOR v = XMLoadFloat3(&dir);
        v = XMVector3Transform(v, XMMatrixRotationRollPitchYaw(ry, rx, 0));
        XMStoreFloat3(&dir, v);
    }
    return dir;
}