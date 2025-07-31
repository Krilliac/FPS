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
#include "InputSystem.h"

namespace SparkEngine {
  bool InputSystem::Initialize(EntityRegistry* r) {
    m_registry = r;
    return true;
  }

  void InputSystem::Update(float dt) {
    // publish input events to ECS
	// TODO: Implement
  }
}