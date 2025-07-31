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
#include "AudioSystem.h"

namespace SparkEngine {
  bool AudioSystem::Initialize(EntityRegistry* r) {
    m_registry = r;
    // set listener at primary camera
	// TODO: Implement
    return true;
  }

  void AudioSystem::Update(float dt) {
    // update listener transform each frame
	// TODO: Implement
  }
}