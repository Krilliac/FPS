#include "AssetManager.h"
#include "../Utils/Logger.h"

namespace SparkEngine {
    template<typename T>
    T* AssetManager::LoadAsset(const std::string& name, const std::string& path) {
        if (m_assets.find(name) != m_assets.end()) {
            Logger::Warning(\"Asset already loaded: \" + name);
            return dynamic_cast<T*>(m_assets[name].get());
        }
        auto asset = std::make_unique<T>();
        if (!asset->Load(path)) {
            Logger::Error(\"Failed to load asset: \" + name);
            return nullptr;
        }
        T* ptr = asset.get();
        m_assets[name] = std::move(asset);
        return ptr;
    }

    template<typename T>
    T* AssetManager::GetAsset(const std::string& name) {
        auto it = m_assets.find(name);
        if (it == m_assets.end()) {
            Logger::Warning(\"Asset not found: \" + name);
            return nullptr;
        }
        return dynamic_cast<T*>(it->second.get());
    }

    void AssetManager::UnloadAll() {
        Logger::Info(\"Unloading all assets\");
        m_assets.clear();
    }

    // Explicit instantiation
    template TextureAsset* AssetManager::LoadAsset<TextureAsset>(const std::string&, const std::string&);
    template MeshAsset*    AssetManager::LoadAsset<MeshAsset>(const std::string&, const std::string&);
    template TextureAsset* AssetManager::GetAsset<TextureAsset>(const std::string&);
    template MeshAsset*    AssetManager::GetAsset<MeshAsset>(const std::string&);
}
