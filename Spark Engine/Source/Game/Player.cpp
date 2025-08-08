#include "Player.h"
#include "Utils/Assert.h"
#include "..\Camera\SparkEngineCamera.h"
#include "..\Input\InputManager.h"
#include "..\Projectiles\WeaponStats.h"
#include "..\Projectiles\ProjectilePool.h"
#include "..\Utils\MathUtils.h"
#include "..\Game\Console.h"
#include "../Utils/ConsoleProcessManager.h"
#include <algorithm>
#include <DirectXMath.h>
#include <cmath>
#include <iostream>

using namespace DirectX;
extern Console g_console;

// Helper macro for logging to external console
#define LOG_TO_CONSOLE(msg, type) Spark::ConsoleProcessManager::GetInstance().Log(msg, type)

// Constructor
Player::Player()
    : m_currentWeapon(GetWeaponStats(WeaponType::PISTOL))
    , m_collisionSphere(GetPosition(), 0.5f)
{
    LOG_TO_CONSOLE(L"Player constructed.", L"INFO");
    SetName("Player");
    m_currentAmmo = m_currentWeapon.MagazineSize;
    ASSERT_MSG(m_currentAmmo > 0, "Initial ammo must be positive");
}

// Initialization
HRESULT Player::Initialize(ID3D11Device* device,
    ID3D11DeviceContext* context,
    SparkEngineCamera* camera,
    InputManager* input)
{
    LOG_TO_CONSOLE(L"Player::Initialize called.", L"OPERATION");
    ASSERT_NOT_NULL(device);
    ASSERT_NOT_NULL(context);
    ASSERT_NOT_NULL(camera);
    ASSERT_NOT_NULL(input);

    m_camera = camera;
    m_input = input;
    SetVisible(false);

    if (!m_projectilePool)
    {
        m_projectilePool = new ProjectilePool(50);
        ASSERT_NOT_NULL(m_projectilePool);
        HRESULT hr = m_projectilePool->Initialize(device, context);
        LOG_TO_CONSOLE(L"Player projectile pool initialized. HR=0x" + std::to_wstring(hr), L"INFO");
        ASSERT_MSG(SUCCEEDED(hr), "ProjectilePool::Initialize failed");
        if (FAILED(hr)) return hr;
    }
    LOG_TO_CONSOLE(L"Player initialization complete.", L"INFO");
    return GameObject::Initialize(device, context);
}

// Per-frame update
void Player::Update(float dt)
{
    LOG_TO_CONSOLE(L"Player::Update called. dt=" + std::to_wstring(dt), L"OPERATION");
    ASSERT_MSG(dt >= 0.0f && std::isfinite(dt), "Delta time must be non-negative and finite");
    if (!IsAlive()) return;

    HandleInput(dt);
    UpdateMovement(dt);
    UpdateCombat(dt);
    UpdatePhysics(dt);
    UpdateAnimation(dt);
    UpdateCollision();
    GameObject::Update(dt);
    LOG_TO_CONSOLE(L"Player update complete.", L"INFO");
}

// No mesh rendering for first-person
void Player::Render(const XMMATRIX&, const XMMATRIX&)
{
    LOG_TO_CONSOLE(L"Player::Render called (no mesh rendering for first-person).", L"OPERATION");
}

// Damage & healing
void Player::TakeDamage(float dmg)
{
    LOG_TO_CONSOLE(L"Player::TakeDamage called. dmg=" + std::to_wstring(dmg), L"OPERATION");
    ASSERT_MSG(dmg >= 0.0f, "Damage must be non-negative");
    if (!IsAlive()) return;

    float absorbed = std::min(dmg * 0.5f, m_armor);
    m_armor -= absorbed;
    m_health = std::max(0.0f, m_health - (dmg - absorbed));
    if (m_health <= 0.0f) SetActive(false);
    LOG_TO_CONSOLE(L"Player took damage. Health=" + std::to_wstring(m_health) + L" Armor=" + std::to_wstring(m_armor), L"INFO");
}

void Player::Heal(float amt)
{
    LOG_TO_CONSOLE(L"Player::Heal called. amt=" + std::to_wstring(amt), L"OPERATION");
    ASSERT_MSG(amt >= 0.0f, "Heal amount must be non-negative");
    m_health = std::min(m_maxHealth, m_health + amt);
    LOG_TO_CONSOLE(L"Player healed. Health=" + std::to_wstring(m_health), L"INFO");
}

void Player::AddArmor(float amt)
{
    LOG_TO_CONSOLE(L"Player::AddArmor called. amt=" + std::to_wstring(amt), L"OPERATION");
    ASSERT_MSG(amt >= 0.0f, "Armor amount must be non-negative");
    m_armor = std::min(m_maxArmor, m_armor + amt);
    LOG_TO_CONSOLE(L"Player armor added. Armor=" + std::to_wstring(m_armor), L"INFO");
}

// Actions
void Player::Jump()
{
    LOG_TO_CONSOLE(L"Player::Jump called.", L"OPERATION");
    if (m_isGrounded && !m_isJumping && m_stamina > 20.0f)
    {
        m_velocity.y = m_jumpHeight;
        m_isJumping = true;
        m_isGrounded = false;
        m_stamina -= 20.0f;
        LOG_TO_CONSOLE(L"Player jumped.", L"INFO");
    }
}

void Player::StartReload()
{
    LOG_TO_CONSOLE(L"Player::StartReload called.", L"OPERATION");
    if (!m_isReloading && m_currentAmmo < m_currentWeapon.MagazineSize)
    {
        m_isReloading = true;
        m_reloadTimer = m_currentWeapon.ReloadTime;
        LOG_TO_CONSOLE(L"Player started reloading.", L"INFO");
    }
}

void Player::Fire()
{
    LOG_TO_CONSOLE(L"Player::Fire called.", L"OPERATION");
    if (m_isReloading || m_currentAmmo <= 0 || m_fireTimer > 0.0f) return;
    ASSERT_NOT_NULL(m_projectilePool);

    XMFLOAT3 pos = m_camera->GetPosition();
    XMFLOAT3 dir = CalculateFireDirection();

    ProjectileType type = ProjectileType::BULLET;
    switch (m_currentWeapon.Type)
    {
    case WeaponType::ROCKET_LAUNCHER:   type = ProjectileType::ROCKET;   break;
    case WeaponType::GRENADE_LAUNCHER:  type = ProjectileType::GRENADE;  break;
    default:                            type = ProjectileType::BULLET;   break;
    }

    m_projectilePool->FireProjectile(type, pos, dir, m_currentWeapon.MuzzleVelocity);

    --m_currentAmmo;
    m_fireTimer = 60.0f / m_currentWeapon.FireRate;
    m_isFiring = true;
    LOG_TO_CONSOLE(L"Player fired weapon. Ammo=" + std::to_wstring(m_currentAmmo), L"INFO");
}

void Player::ChangeWeapon(WeaponType t)
{
    LOG_TO_CONSOLE(L"Player::ChangeWeapon called. type=" + std::to_wstring(static_cast<int>(t)), L"OPERATION");
    if (m_isReloading) return;
    m_currentWeapon = GetWeaponStats(t);
    m_currentAmmo = m_currentWeapon.MagazineSize;
    m_fireTimer = 0.0f;
    LOG_TO_CONSOLE(L"Player weapon changed. Ammo=" + std::to_wstring(m_currentAmmo), L"INFO");
}

// Hit callbacks
void Player::OnHit(GameObject* target)
{
    LOG_TO_CONSOLE(L"Player::OnHit called.", L"OPERATION");
    ASSERT_NOT_NULL(target);
    TakeDamage(m_currentWeapon.Damage);
    LOG_TO_CONSOLE(L"Player hit target.", L"INFO");
}

void Player::OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal)
{
    LOG_TO_CONSOLE(L"Player::OnHitWorld called.", L"OPERATION");
    ASSERT_MSG(std::isfinite(hitPoint.x) && std::isfinite(hitPoint.y) && std::isfinite(hitPoint.z),
        "Invalid hitPoint");
    TakeDamage(m_currentWeapon.Damage);
    LOG_TO_CONSOLE(L"Player hit world.", L"INFO");
}

// Input handling
void Player::HandleInput(float)
{
    LOG_TO_CONSOLE(L"Player::HandleInput called.", L"OPERATION");
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
    LOG_TO_CONSOLE(L"Player input handled.", L"INFO");
}

// Movement logic
void Player::UpdateMovement(float dt)
{
    LOG_TO_CONSOLE(L"Player::UpdateMovement called. dt=" + std::to_wstring(dt), L"OPERATION");
    if (!m_camera || !m_input) return;
    float speed = m_speed;
    if (m_isRunning && m_stamina > 0.0f)
    {
        speed *= 2.0f; m_stamina = std::max(0.0f, m_stamina - 30.0f * dt);
    }
    else if (m_isCrouching)
    {
        speed *= 0.5f;
    }
    speed *= dt;

    if (m_input->IsKeyDown('W')) m_camera->MoveForward(speed);
    if (m_input->IsKeyDown('S')) m_camera->MoveForward(-speed);
    if (m_input->IsKeyDown('A')) m_camera->MoveRight(-speed);
    if (m_input->IsKeyDown('D')) m_camera->MoveRight(speed);

    if (!m_isRunning)
        m_stamina = std::min(m_maxStamina, m_stamina + 50.0f * dt);

    SetPosition(m_camera->GetPosition());
    LOG_TO_CONSOLE(L"Player movement updated.", L"INFO");
}

// Combat timers
void Player::UpdateCombat(float dt)
{
    LOG_TO_CONSOLE(L"Player::UpdateCombat called. dt=" + std::to_wstring(dt), L"OPERATION");
    if (m_fireTimer > 0.0f) { m_fireTimer -= dt; if (m_fireTimer < 0) m_fireTimer = 0; }
    if (m_isReloading)
    {
        m_reloadTimer -= dt;
        if (m_reloadTimer <= 0.0f)
        {
            m_currentAmmo = m_currentWeapon.MagazineSize;
            m_isReloading = false;
        }
    }
    LOG_TO_CONSOLE(L"Player combat updated.", L"INFO");
}

// Physics & gravity
void Player::UpdatePhysics(float dt)
{
    LOG_TO_CONSOLE(L"Player::UpdatePhysics called. dt=" + std::to_wstring(dt), L"OPERATION");
    if (!m_isGrounded) ApplyGravity(dt);
    CheckGroundCollision();

    XMFLOAT3 pos = GetPosition();
    pos.x += m_velocity.x * dt;
    pos.y += m_velocity.y * dt;
    pos.z += m_velocity.z * dt;
    SetPosition(pos);
    if (m_camera) m_camera->SetPosition(pos);

    m_velocity.x *= 0.9f;
    m_velocity.z *= 0.9f;
    LOG_TO_CONSOLE(L"Player physics updated.", L"INFO");
}

// Animation (head bob & footsteps)
void Player::UpdateAnimation(float dt)
{
    LOG_TO_CONSOLE(L"Player::UpdateAnimation called. dt=" + std::to_wstring(dt), L"OPERATION");
    if (m_input && (m_input->IsKeyDown('W') || m_input->IsKeyDown('A') ||
        m_input->IsKeyDown('S') || m_input->IsKeyDown('D')))
    {
        m_bobTimer += dt * 8.0f;
        HandleFootsteps(dt);
    }
    LOG_TO_CONSOLE(L"Player animation updated.", L"INFO");
}

// Collision sphere update
void Player::UpdateCollision()
{
    LOG_TO_CONSOLE(L"Player::UpdateCollision called.", L"OPERATION");
    m_collisionSphere.Center = GetPosition();
    LOG_TO_CONSOLE(L"Player collision updated.", L"INFO");
}

// Apply gravity
void Player::ApplyGravity(float dt)
{
    LOG_TO_CONSOLE(L"Player::ApplyGravity called. dt=" + std::to_wstring(dt), L"OPERATION");
    m_velocity.y += -20.0f * dt;
    if (m_velocity.y < -50.0f) m_velocity.y = -50.0f;
    LOG_TO_CONSOLE(L"Player gravity applied.", L"INFO");
}

// Ground check
void Player::CheckGroundCollision()
{
    LOG_TO_CONSOLE(L"Player::CheckGroundCollision called.", L"OPERATION");
    XMFLOAT3 pos = GetPosition();
    if (pos.y <= 0.0f && m_velocity.y <= 0.0f)
    {
        pos.y = 0.0f; SetPosition(pos);
        m_velocity.y = 0.0f;
        m_isGrounded = true; m_isJumping = false;
        if (m_camera) m_camera->SetPosition(pos);
    }
    LOG_TO_CONSOLE(L"Player ground collision checked.", L"INFO");
}

// Footsteps
void Player::HandleFootsteps(float dt)
{
    LOG_TO_CONSOLE(L"Player::HandleFootsteps called. dt=" + std::to_wstring(dt), L"OPERATION");
    m_footstepTimer += dt;
    float interval = m_isRunning ? 0.3f : 0.6f;
    if (m_footstepTimer >= interval)
    {
        // footstep SFX
        m_footstepTimer = 0.0f;
    }
    LOG_TO_CONSOLE(L"Player footsteps handled.", L"INFO");
}

// Weapon defaults lookup
WeaponStats Player::GetWeaponStats(WeaponType type)
{
    return GetDefaultWeaponStats(type);
}

// Fire direction w/spread
XMFLOAT3 Player::CalculateFireDirection()
{
    if (!m_camera) return XMFLOAT3(0, 0, 1);
    XMFLOAT3 f = m_camera->GetForward();
    float spread = (1.0f - m_currentWeapon.Accuracy) * 0.1f;
    if (spread > 0.0f)
    {
        f.x += MathUtils::RandomFloat(-spread, spread);
        f.y += MathUtils::RandomFloat(-spread, spread);
        XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&f));
        XMStoreFloat3(&f, v);
    }
    return f;
}