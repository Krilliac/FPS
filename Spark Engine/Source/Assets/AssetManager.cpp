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
#include "AssetManager.h"
#include <iostream>

namespace SparkEngine {
    uint64_t Asset::s_nextAssetID = 0;

    AssetManager::~AssetManager() {
        Shutdown();
    }

    bool AssetManager::Initialize(size_t threads) {
        m_running = true;
        for (size_t i=0; i<threads; i++) {
            m_loadThreads.emplace_back(&AssetManager::LoaderThreadFunction, this);
        }
        return true;
    }

    void AssetManager::Shutdown() {
        { 
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_running = false;
        }
        m_queueCondition.notify_all();
        for (auto& t : m_loadThreads) if (t.joinable()) t.join();
        m_loadThreads.clear();
        m_assets.clear();
    }

    void AssetManager::LoaderThreadFunction() {
        while (m_running) {
            std::unique_ptr<AssetLoadJob> job;
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_queueCondition.wait(lock, [this]{return !m_loadQueue.empty()||!m_running;});
                if (!m_running) break;
                job = std::move(m_loadQueue.front());
                m_loadQueue.pop();
            }
            if (job->asset->Load(job->filepath)) {
                m_assets[job->filepath] = job->asset;
                m_totalMemoryUsage += job->asset->GetMemoryUsage();
                m_loadedAssetCount++;
                job->promise.set_value(true);
                if (job->callback) { job->callback(job->asset); }
            } else {
                job->promise.set_value(false);
            }
        }
    }
}