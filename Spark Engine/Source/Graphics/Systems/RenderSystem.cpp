#include <imgui.h>
#include "Utils/FileWatcher.h"
#include "Utils/Assert.h"
#include "Utils/CrashHandler.h"
#include "Assets/AssetSystem.h"
#include "Editor/EditorSystem.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Scripting/ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/Systems/RenderSystem.h"
#include "Game/SparkEngineGame.h"
#include "RenderSystem.h"
#include "../RenderThreadManager.h"

namespace SparkEngine {
  RenderSystem::RenderSystem(GraphicsEngine* g) : m_graphics(g) {}

  void RenderSystem::Initialize(EntityRegistry* r) { m_registry = r; }

  void RenderSystem::Update(float dt) {
    auto view = m_registry->View<MeshRendererComponent, TransformComponent>();
    for (auto e : view) {
      auto& mesh = view.get<MeshRendererComponent>(e);
      auto& xf   = view.get<TransformComponent>(e);
      m_graphics->DrawMesh(mesh.meshPath, xf.GetWorldMatrix());
    }
  }
}