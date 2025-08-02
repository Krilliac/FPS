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
#include "CubeObject.h"
#include "Utils/Assert.h"          // custom assert
#include <DirectXMath.h>
#include <string>

using XMFLOAT3;
using XMMATRIX;

CubeObject::CubeObject(float size)
    : m_size(size)
{
    SPARK_ASSERT_MSG(size > 0.0f, "Cube size must be positive");
    SetName("Cube_" + std::to_string(GetID()));
}

HRESULT CubeObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    SPARK_ASSERT(device != nullptr);
    SPARK_ASSERT(context != nullptr);
    return GameObject::Initialize(device, context);
}

void CubeObject::CreateMesh()
{
    // Generate primitive cube mesh data
    auto md = Primitives::CreateCube(m_size);
    SPARK_ASSERT_MSG(!md.vertices.empty() && !md.indices.empty(), "Primitive cube mesh is empty");

    // Load model from file or placeholder if missing
    LoadOrPlaceholderMesh(*m_mesh, m_modelPath);

    // Ensure mesh was assigned
    SPARK_ASSERT(m_mesh != nullptr);
}

