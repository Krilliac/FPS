#pragma once
#include "../Core/framework.h"
#include "IAsset.h"
#include <unordered_map>

namespace SparkEngine {
    class AssetManager {
    public:
        AssetManager() = default;
        ~AssetManager() { UnloadAll(); }

        template<typename T>
        T* LoadAsset(const std::string& name, const std::string& path);

        template<typename T>
        T* GetAsset(const std::string& name);

        void UnloadAll();

    private:
        std::unordered_map<std::string, std::unique_ptr<IAsset>> m_assets;
    };
}
