#include "Rocket.h"
#include "..\Utils\MathUtils.h"  // if you need random effects

Rocket::Rocket()
    : m_explosionRadius(5.0f)
    , m_hasExploded(false)
{
    m_damage      = 75.0f;
    m_speed       = 30.0f;
    m_maxLifeTime = 10.0f;
    SetGravity(true, 0.3f);
    SetScale({ 0.2f, 0.2f, 0.8f });
}

HRESULT Rocket::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return Projectile::Initialize(device, context);
}

void Rocket::Update(float deltaTime)
{
    Projectile::Update(deltaTime);
    // Optionally add trail effects here
}

void Rocket::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    Projectile::Render(view, projection);
}

void Rocket::OnHit(GameObject* target)
{
    if (!m_hasExploded)
        Explode(GetPosition());
}

void Rocket::OnHitWorld(const XMFLOAT3& hitPoint, const XMFLOAT3& normal)
{
    if (!m_hasExploded)
        Explode(hitPoint);
}

void Rocket::Explode(const XMFLOAT3& position)
{
    m_hasExploded = true;

    // TODO: spawn explosion effect at `position`
    // TODO: apply area damage to nearby objects

    Deactivate();
}
