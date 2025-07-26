#include "Bullet.h"

Bullet::Bullet()
{
    // Customize bullet parameters
    m_damage      = 15.0f;
    m_speed       = 100.0f;
    m_maxLifeTime = 3.0f;
    SetScale({ 0.05f, 0.05f, 0.2f });
}

HRESULT Bullet::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    // Base initialization sets up mesh and transforms
    return Projectile::Initialize(device, context);
}

void Bullet::Update(float deltaTime)
{
    // Use base physics/lifetime/collision
    Projectile::Update(deltaTime);
}

void Bullet::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    Projectile::Render(view, projection);
}