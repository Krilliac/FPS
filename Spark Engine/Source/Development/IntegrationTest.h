#pragma once
#include "../ECS/EntityRegistry.h"
#include "../Assets/AssetManager.h"
#include "../Scripting/ScriptingSystem.h"
#include "../Graphics/GraphicsEngine.h"

namespace SparkEngine {
    bool RunCollaborativeIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                       ScriptingSystem* scripting, GraphicsEngine* graphics);
}
