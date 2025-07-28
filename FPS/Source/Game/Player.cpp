#include "Player.h"
#include "..\Camera\FPSCamera.h"
#include "..\Input\InputManager.h"
#include "..\Projectiles\ProjectilePool.h"
#include "..\Utils\MathUtils.h"
#include <algorithm>
#include <DirectXMath.h>

using namespace DirectX;
extern Console g_console;

/* Constructor */
Player::Player()
    : m_currentWeapon(GetWeaponStats(WeaponType::PISTOL)),
    m_collisionSphere(GetPosition(), 0.5f)
{
    SetName("Player");
    m_currentAmmo = m_currentWeapon.MagazineSize;
}

/* Initialization */
HRESULT Player::Initialize(ID3D11Device* device,
    ID3D11DeviceContext* context,
    FPSCamera* camera,
    InputManager* input)
{
    m_camera = camera;
    m_input = input;
    SetVisible(false);

    // Ensure projectile pool is a raw pointer
    if (!m_projectilePool)
    {
        m_projectilePool = new ProjectilePool(50);
        m_projectilePool->Initialize(device, context);
    }

    return GameObject::Initialize(device, context);
}

/* Update */
void Player::Update(float dt)
{
    if (!IsAlive()) return;
    HandleInput(dt);
    UpdateMovement(dt);
    UpdateCombat(dt);
    UpdatePhysics(dt);
    UpdateAnimation(dt);
    UpdateCollision();
    GameObject::Update(dt);
}

/* Render – no mesh first-person */
void Player::Render(const XMMATRIX&, const XMMATRIX&) {}

/* Damage & healing */
void Player::TakeDamage(float dmg)
{
    float absorbed = std::min(dmg * 0.5f, m_armor);
    m_armor -= absorbed;
    m_health = std::max(0.0f, m_health - (dmg - absorbed));
}
void Player::Heal(float amt) { m_health = std::min(m_maxHealth, m_health + amt); }
void Player::AddArmor(float amt) { m_armor = std::min(m_maxArmor, m_armor + amt); }

/* Input handling */
void Player::HandleInput(float)
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

/* Movement */
void Player::UpdateMovement(float dt)
{
    if (!m_input || !m_camera) return;

    XMFLOAT3 move{ 0,0,0 };
    if (m_input->IsKeyDown('W')) move.z += 1;
    if (m_input->IsKeyDown('S')) move.z -= 1;
    if (m_input->IsKeyDown('A')) move.x -= 1;
    if (m_input->IsKeyDown('D')) move.x += 1;

    float speed = m_speed;
    if (m_isRunning && m_stamina > 0)
    {
        speed *= 2.0f;
        m_stamina -= 30.0f * dt;
    }
    else if (m_isCrouching)
    {
        speed *= 0.5f;
    }

    if (move.x || move.z)
    {
        XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&move));
        XMStoreFloat3(&move, v);
        auto fw = m_camera->GetForward();
        XMFLOAT3 rt{ 1,0,0 };

        Translate({
            (rt.x * move.x + fw.x * move.z) * speed * dt,
            0,
            (rt.z * move.x + fw.z * move.z) * speed * dt
            });
        HandleFootsteps(dt);
    }

    if (!m_isRunning)
        m_stamina = std::min(m_maxStamina, m_stamina + 50.0f * dt);
}

/* Combat */
void Player::StartReload()
{
    if (!m_isReloading &&
        m_currentAmmo < m_currentWeapon.MagazineSize)
    {
        m_isReloading = true;
        m_reloadTimer = m_currentWeapon.ReloadTime;
    }
}

void Player::Fire()
{
    if (m_isReloading || m_currentAmmo <= 0 ||
        m_fireTimer > 0.0f || !m_projectilePool || !m_camera)
        return;

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
    default: break;
    }

    --m_currentAmmo;
    m_fireTimer = 1.0f / m_currentWeapon.FireRate;
    m_isFiring = true;
}

void Player::UpdateCombat(float dt)
{
    if (m_fireTimer > 0.0f)
    {
        m_fireTimer -= dt;
        if (m_fireTimer <= 0.0f) m_isFiring = false;
    }
    if (m_isReloading)
    {
        m_reloadTimer -= dt;
        if (m_reloadTimer <= 0.0f)
        {
            m_currentAmmo = m_currentWeapon.MagazineSize;
            m_isReloading = false;
        }
    }
}

/* Physics */
void Player::ApplyGravity(float dt)
{
    if (!m_isGrounded)
        m_velocity.y -= 9.8f * dt;
}

void Player::CheckGroundCollision()
{
    auto p = GetPosition();
    if (p.y <= 0.0f)
    {
        p.y = 0.0f;
        m_velocity.y = 0.0f;
        m_isGrounded = true;
        m_isJumping = false;
        SetPosition(p);
    }
    else m_isGrounded = false;
}

void Player::UpdatePhysics(float dt)
{
    ApplyGravity(dt);
    CheckGroundCollision();
    Translate({ m_velocity.x * dt,
                m_velocity.y * dt,
                m_velocity.z * dt });
    if (m_camera)
    {
        XMFLOAT3 eye{ 0,1.7f,0 };
        if (m_isCrouching) eye.y *= 0.6f;
        auto pos = GetPosition();
        pos.x += eye.x; pos.y += eye.y; pos.z += eye.z;
        m_camera->SetPosition(pos);
    }
}

/* Jump */
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

/* Change weapon */
void Player::ChangeWeapon(WeaponType type)
{
    m_currentWeapon = GetWeaponStats(type);
    m_currentAmmo = m_currentWeapon.MagazineSize;
    m_isReloading = false;
    m_fireTimer = 0.0f;
}

/* Animation */
void Player::UpdateAnimation(float dt)
{
    bool moving = m_input &&
        (m_input->IsKeyDown('W') || m_input->IsKeyDown('A') ||
            m_input->IsKeyDown('S') || m_input->IsKeyDown('D'));
    if (moving) m_bobTimer += dt * 8.0f;
    else        m_bobTimer = 0.0f;
}

/* Collision update */
void Player::UpdateCollision()
{
    m_collisionSphere.Center = GetPosition();
}

/* Footsteps */
void Player::HandleFootsteps(float dt)
{
    m_footstepTimer -= dt;
    if (m_footstepTimer <= 0.0f)
        m_footstepTimer = m_isRunning ? 0.3f : 0.6f;
}

/* Weapon stats */
WeaponStats Player::GetWeaponStats(WeaponType t)
{
    switch (t)
    {
    case WeaponType::PISTOL:          return WeaponStats(20, 3, 12, 1.5f, 50, 0.9f, t);
    case WeaponType::RIFLE:           return WeaponStats(35, 8, 30, 2.0f, 100, 0.95f, t);
    case WeaponType::SHOTGUN:         return WeaponStats(60, 1.5f, 8, 3.0f, 25, 0.7f, t);
    case WeaponType::ROCKET_LAUNCHER: return WeaponStats(150, 0.5f, 1, 4.0f, 200, 0.95f, t);
    case WeaponType::GRENADE_LAUNCHER:return WeaponStats(100, 1.0f, 6, 3.5f, 80, 0.8f, t);
    default:                          return WeaponStats();
    }
}

/* Calculate firing direction */
XMFLOAT3 Player::CalculateFireDirection()
{
    XMFLOAT3 dir = m_camera ? m_camera->GetForward()
        : XMFLOAT3{ 0,0,1 };
    float spread = 1.0f - m_currentWeapon.Accuracy;
    if (spread > 0.0f)
    {
        float a = spread * 0.1f;
        float rx = MathUtils::RandomFloat(-a, a);
        float ry = MathUtils::RandomFloat(-a, a);
        auto v = XMLoadFloat3(&dir);
        v = XMVector3Transform(v,
            XMMatrixRotationRollPitchYaw(ry, rx, 0));
        XMStoreFloat3(&dir, v);
    }
    return dir;
}