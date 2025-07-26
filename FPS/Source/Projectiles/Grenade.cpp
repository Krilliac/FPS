#include "Grenade.h"

Grenade::Grenade()
    : m_fuseTime(3.0f)
    , m_explosionRadius(8.0f)
    , m_hasExploded(false)
{
    m_damage      = 100.0f;
    m_speed       = 15.0f;
    m_maxLifeTime = 5.0f;
    SetGravity(true, 1.0f);
    SetScale({ 0.3f, 0.3f, 0.3f });
}

HRESULT Grenade::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    return Projectile::Initialize(device, context);
}

void Grenade::Update(float deltaTime)
{
    if (!m_active) return;

    // Fuse timer
    m_lifeTime += deltaTime;
    if (m_lifeTime >= m_fuseTime && !m_hasExploded)
    {
        Explode();
        return;
    }

    Projectile::Update(deltaTime);
}

void Grenade::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    Projectile::Render(view, projection);
}

void Grenade::Explode()
{
    m_hasExploded = true;

    // TODO: spawn explosion effect at current position
    // TODO: apply area damage using m_explosionRadius

    Deactivate();
}