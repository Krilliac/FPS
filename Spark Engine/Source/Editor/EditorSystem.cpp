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
#include "EditorSystem.h"

namespace SparkEngine {
  bool EditorSystem::Initialize(EntityRegistry* r) {
    m_registry = r;
    return true;
  }

  void EditorSystem::Update(float dt) {
    m_editorManager->NewFrame();
    m_editorManager->Render();
    m_editorManager->Present();
  }
}












