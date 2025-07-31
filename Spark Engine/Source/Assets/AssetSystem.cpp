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
#include "AssetSystem.h"

namespace SparkEngine {
  bool AssetSystem::Initialize(EntityRegistry* r) {
    m_registry = r;
    m_assetManager->Initialize();
    return true;
  }

  void AssetSystem::Update(float dt) {
    // process async load completions
	// TODO: Implement
  }
}