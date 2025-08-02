#include <imgui.h>
#include "Utils/FileWatcher.h"
#include "Utils/CrashHandler.h"
#include "Assets/AssetSystem.h"
#include "Editor/EditorSystem.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Scripting/ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/Systems/RenderSystem.h"
#include "Game/SparkEngineGame.h"
// Grenade.cpp
#include "Grenade.h"
#include "Utils/Assert.h"

using XMMATRIX;
using XMFLOAT3;

Grenade::Grenade()
    : m_fuseTime(3.0f)
    , m_explosionRadius(8.0f)
    , m_hasExploded(false)
{
    // Validate parameters
    SPARK_ASSERT_MSG(m_fuseTime > 0.0f, "Grenade fuse time must be positive");
    SPARK_ASSERT_MSG(m_explosionRadius > 0.0f, "Grenade explosion radius must be positive");

    m_damage = 100.0f;
    m_speed = 15.0f;
    m_maxLifeTime = 5.0f;

    // Enable gravity
    SetGravity(true, 1.0f);

    // Scale grenade
    XMFLOAT3 scale{ 0.3f, 0.3f, 0.3f };
    SPARK_ASSERT_MSG(scale.x > 0.0f && scale.y > 0.0f && scale.z > 0.0f, "Grenade scale must be positive");
    SetScale(scale);
}

HRESULT Grenade::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    SPARK_ASSERT_MSG(device != nullptr, "Grenade::Initialize device is null");
    SPARK_ASSERT_MSG(context != nullptr, "Grenade::Initialize context is null");

    HRESULT hr = Projectile::Initialize(device, context);
    SPARK_ASSERT_MSG(SUCCEEDED(hr), "Projectile::Initialize failed in Grenade");
    return hr;
}

void Grenade::Update(float deltaTime)
{
    SPARK_ASSERT_MSG(deltaTime >= 0.0f && std::isfinite(deltaTime), "Invalid deltaTime in Grenade::Update");

    if (!m_active) return;

    // Fuse countdown
    m_lifeTime += deltaTime;
    if (m_lifeTime >= m_fuseTime && !m_hasExploded)
    {
        Explode();
        return;
    }

    // Use base physics/lifetime/collision
    Projectile::Update(deltaTime);
}

void Grenade::Render(const XMMATRIX& view, const XMMATRIX& projection)
{
    SPARK_ASSERT_MSG(m_mesh != nullptr, "Grenade mesh not initialized");
    Projectile::Render(view, projection);
}

void Grenade::Explode()
{
    SPARK_ASSERT_MSG(!m_hasExploded, "Grenade exploded multiple times");
    m_hasExploded = true;

    // TODO: spawn explosion effect at current position
    // TODO: apply area damage using m_explosionRadius

    Deactivate();
}

