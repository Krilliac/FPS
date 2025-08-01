#include "AssetManager.h"
#include "DynamicAssetPipeline.h"
#include "StreamingAssetManager.h"
#include "../Utils/Assert.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace SparkEngine {
    uint64_t Asset::s_nextAssetID = 1;

    AssetManager::AssetManager() {
        m_threadPool = std::make_unique<ThreadPool>(4); // 4 worker threads
    }

    AssetManager::~AssetManager() {
        Shutdown();
    }

    bool AssetManager::Initialize() {
        // Register built-in asset types
        RegisterAssetType<TextureAsset>();
        RegisterAssetType<MeshAsset>();
        RegisterAssetType<MaterialAsset>();
        RegisterAssetType<ShaderAsset>();
        RegisterAssetType<AudioAsset>();
        RegisterAssetType<SceneAsset>();

        // Initialize async loading thread pool
        if (!m_threadPool) {
            std::cerr << "Failed to create thread pool for asset loading" << std::endl;
            return false;
        }

        // Set up asset directories
        SetAssetDirectory("Assets/");
        
        std::cout << "AssetManager initialized successfully" << std::endl;
        return true;
    }

    void AssetManager::Shutdown() {
        // Wait for all async operations to complete
        if (m_threadPool) {
            m_threadPool->shutdown();
        }

        // Clear all loaded assets
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            m_loadedAssets.clear();
            m_assetFactories.clear();
        }

        // Shutdown collaborative features
        m_dynamicPipeline.reset();
        m_streamingManager.reset();

        std::cout << "AssetManager shutdown complete" << std::endl;
    }

    void AssetManager::Update() {
        // Process completed async loads
        ProcessCompletedLoads();

        // Update streaming and dynamic pipeline
        if (m_streamingManager) {
            m_streamingManager->Update();
        }
        if (m_dynamicPipeline) {
            m_dynamicPipeline->Update();
        }

        // Clean up unreferenced assets periodically
        static float lastCleanup = 0.0f;
        float currentTime = GetTickCount64() / 1000.0f;
        if (currentTime - lastCleanup > 30.0f) { // Every 30 seconds
            CleanupUnreferencedAssets();
            lastCleanup = currentTime;
        }
    }

    std::shared_ptr<Asset> AssetManager::LoadAsset(const std::string& path) {
        // Check if already loaded
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_loadedAssets.find(path);
            if (it != m_loadedAssets.end()) {
                if (auto asset = it->second.lock()) {
                    return asset;
                }
                // Asset was freed, remove from map
                m_loadedAssets.erase(it);
            }
        }

        // Get file extension to determine asset type
        std::string extension = GetFileExtension(path);
        auto factoryIt = m_assetFactories.find(extension);
        if (factoryIt == m_assetFactories.end()) {
            std::cerr << "No factory registered for file type: " << extension << std::endl;
            return nullptr;
        }

        // Load asset
        auto asset = factoryIt->second->LoadAsset(path);
        if (asset) {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            m_loadedAssets[path] = asset;
        }

        return asset;
    }

    std::future<std::shared_ptr<Asset>> AssetManager::LoadAssetAsync(const std::string& path) {
        auto promise = std::make_shared<std::promise<std::shared_ptr<Asset>>>();
        auto future = promise->get_future();

        // Check if already loaded first
        {
            std::lock_guard<std::mutex> lock(m_assetMutex);
            auto it = m_loadedAssets.find(path);
            if (it != m_loadedAssets.end()) {
                if (auto asset = it->second.lock()) {
                    promise->set_value(asset);
                    return future;
                }
                m_loadedAssets.erase(it);
            }
        }

        // Submit async load task
        m_threadPool->enqueue([this, path, promise]() {
            try {
                auto asset = LoadAsset(path);
                promise->set_value(asset);
            } catch (const std::exception& e) {
                promise->set_exception(std::current_exception());
            }
        });

        return future;
    }

    void AssetManager::UnloadAsset(const std::string& path) {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        m_loadedAssets.erase(path);
    }

    void AssetManager::ReloadAsset(const std::string& path) {
        UnloadAsset(path);
        LoadAsset(path);
    }

    bool AssetManager::IsAssetLoaded(const std::string& path) const {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        auto it = m_loadedAssets.find(path);
        return (it != m_loadedAssets.end() && !it->second.expired());
    }

    std::vector<std::string> AssetManager::GetLoadedAssetPaths() const {
        std::vector<std::string> paths;
        std::lock_guard<std::mutex> lock(m_assetMutex);
        
        for (const auto& [path, weakAsset] : m_loadedAssets) {
            if (!weakAsset.expired()) {
                paths.push_back(path);
            }
        }
        
        return paths;
    }

    void AssetManager::EnableDynamicPipeline(bool enable) {
        if (enable && !m_dynamicPipeline) {
            m_dynamicPipeline = std::make_unique<DynamicAssetPipeline>(this, nullptr);
            m_dynamicPipeline->Initialize();
        } else if (!enable && m_dynamicPipeline) {
            m_dynamicPipeline->Shutdown();
            m_dynamicPipeline.reset();
        }
    }

    void AssetManager::EnableAssetStreaming(bool enable) {
        if (enable && !m_streamingManager) {
            m_streamingManager = std::make_unique<StreamingAssetManager>(this, nullptr);
            m_streamingManager->Initialize();
        } else if (!enable && m_streamingManager) {
            m_streamingManager->Shutdown();
            m_streamingManager.reset();
        }
    }

    void AssetManager::NotifyAssetModified(const std::string& assetId) {
        if (m_dynamicPipeline) {
            m_dynamicPipeline->NotifyAssetModified(assetId);
        }
        
        // Reload the asset
        ReloadAsset(assetId);
    }

    void AssetManager::RequestAssetStream(const std::string& assetId, float priority) {
        if (m_streamingManager) {
            m_streamingManager->RequestAssetStream(assetId, priority);
        }
    }

    void AssetManager::ProcessCompletedLoads() {
        // This would process any completed async loads
        // Implementation depends on the threading model
    }

    void AssetManager::CleanupUnreferencedAssets() {
        std::lock_guard<std::mutex> lock(m_assetMutex);
        
        auto it = m_loadedAssets.begin();
        while (it != m_loadedAssets.end()) {
            if (it->second.expired()) {
                it = m_loadedAssets.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::string AssetManager::GetFileExtension(const std::string& path) const {
        size_t lastDot = path.find_last_of('.');
        if (lastDot != std::string::npos) {
            std::string ext = path.substr(lastDot);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            return ext;
        }
        return "";
    }

    // TextureAsset implementation
    bool TextureAsset::LoadFromFile(const std::string& path) {
        // Implementation would use stb_image or similar
        std::cout << "Loading texture: " << path << std::endl;
        m_path = path;
        return true; // Simplified
    }

    // MeshAsset implementation  
    bool MeshAsset::LoadFromFile(const std::string& path) {
        // Implementation would use assimp or similar
        std::cout << "Loading mesh: " << path << std::endl;
        m_path = path;
        return true; // Simplified
    }

    // Asset factories
    std::shared_ptr<Asset> TextureFactory::LoadAsset(const std::string& path) {
        auto texture = std::make_shared<TextureAsset>();
        if (texture->LoadFromFile(path)) {
            return texture;
        }
        return nullptr;
    }

    std::shared_ptr<Asset> MeshFactory::LoadAsset(const std::string& path) {
        auto mesh = std::make_shared<MeshAsset>();
        if (mesh->LoadFromFile(path)) {
            return mesh;
        }
        return nullptr;
    }
}
