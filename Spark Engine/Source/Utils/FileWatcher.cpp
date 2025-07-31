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
#include "FileWatcher.h"
#include <filesystem>

namespace SparkEngine {
    FileWatcher::FileWatcher(const std::string& dir) : m_dir(dir) {}
    void FileWatcher::Update() {
        for (auto& p : std::filesystem::recursive_directory_iterator(m_dir)) {
            auto f = p.path().string();
            auto tp = std::filesystem::last_write_time(p);
            if (m_times[f] != tp) {
                m_times[f] = tp;
                OnModified(f);
            }
        }
    }
}