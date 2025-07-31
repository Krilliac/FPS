#pragma once
#include "../Core/framework.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <functional>
#include <future>
#include <thread>
#include <queue>
#include <mutex>

namespace SparkEngine {
    // Asset base class
    class Asset {
    public:
        virtual ~Asset() = default;
        virtual bool Load(const std::string& filepath) = 0;
        virtual void Unload() = 0;
        virtual bool IsLoaded() const { return m_loaded; }
        virtual size_t GetMemoryUsage() const { return 0; }
        
        const std::string& GetFilePath() const { return m_filepath; }
        const std::string& GetName() const { return m_name; }
        
        uint64_t GetAssetID() const { return m_assetID; }
        
    protected:
        std::string m_filepath;
        std::string m_name;
        bool m_loaded = false;
        uint64_t m_assetID = 0;
        
        static uint64_t s_nextAssetID;
        
        Asset() : m_assetID(++s_nextAssetID) {}
    };

    // Asset loading job
    struct AssetLoadJob {
        std::string filepath;
        std::shared_ptr<Asset> asset;
        std::promise<bool> promise;
        std::function<void(std::shared_ptr<Asset>)> callback;
        
        AssetLoadJob(const std::string& path, std::shared_ptr<Asset> assetPtr)
            : filepath(path), asset(assetPtr) {}
    };

    class AssetManager {
    private:
        std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets;
        std::unordered_map<std::type_index, std::function<std::shared_ptr<Asset>()>> m_assetFactories;
        
        // Async loading
        std::queue<std::unique_ptr<AssetLoadJob>> m_loadQueue;
        std::vector<std::thread> m_loadThreads;
        std::mutex m_queueMutex;
        std::condition_variable m_queueCondition;
        std::atomic<bool> m_running{true};
        
        // Statistics
        size_t m_totalMemoryUsage = 0;
        size_t m_loadedAssetCount = 0;
        
        static constexpr size_t DEFAULT_THREAD_COUNT = 4;

    public:
        AssetManager() = default;
        ~AssetManager();

        bool Initialize(size_t threadCount = DEFAULT_THREAD_COUNT);
        void Shutdown();

        // Asset type registration
        template<typename T>
        void RegisterAssetType() {
            static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
            
            m_assetFactories[std::type_index(typeid(T))] = []() -> std::shared_ptr<Asset> {
                return std::make_shared<T>();
            };
        }

        // Synchronous loading
        template<typename T>
        std::shared_ptr<T> LoadAsset(const std::string& filepath) {
            static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
            
            // Check if already loaded
            auto it = m_assets.find(filepath);
            if (it != m_assets.end()) {
                return std::static_pointer_cast<T>(it->second);
            }

            // Create new asset
            auto factoryIt = m_assetFactories.find(std::type_index(typeid(T)));
            if (factoryIt == m_assetFactories.end()) {
                return nullptr;
            }

            auto asset = std::static_pointer_cast<T>(factoryIt->second());
            if (asset->Load(filepath)) {
                m_assets[filepath] = asset;
                m_totalMemoryUsage += asset->GetMemoryUsage();
                m_loadedAssetCount++;
                return asset;
            }

            return nullptr;
        }

        // Asynchronous loading
        template<typename T>
        std::future<std::shared_ptr<T>> LoadAssetAsync(const std::string& filepath) {
            static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
            
            auto job = std::make_unique<AssetLoadJob>(filepath, nullptr);
            auto future = job->promise.get_future();
            
            // Check if already loaded
            {
                auto it = m_assets.find(filepath);
                if (it != m_assets.end()) {
                    auto asset = std::static_pointer_cast<T>(it->second);
                    job->promise.set_value(true);
                    return std::async(std::launch::deferred, [asset]() { return asset; });
                }
            }
            
            // Create asset
            auto factoryIt = m_assetFactories.find(std::type_index(typeid(T)));
            if (factoryIt == m_assetFactories.end()) {
                job->promise.set_value(false);
                return std::async(std::launch::deferred, []() -> std::shared_ptr<T> { return nullptr; });
            }
            
            job->asset = factoryIt->second();
            
            // Queue for loading
            {
                std::lock_guard<std::mutex> lock(m_queueMutex);
                m_loadQueue.push(std::move(job));
            }
            m_queueCondition.notify_one();
            
            return std::async(std::launch::deferred, [future = std::move(future), filepath, this]() -> std::shared_ptr<T> {
                if (future.get()) {
                    auto it = m_assets.find(filepath);
                    if (it != m_assets.end()) {
                        return std::static_pointer_cast<T>(it->second);
                    }
                }
                return nullptr;
            });
        }

        // Asset management
        template<typename T>
        std::shared_ptr<T> GetAsset(const std::string& filepath) {
            auto it = m_assets.find(filepath);
            if (it != m_assets.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        void UnloadAsset(const std::string& filepath);
        void UnloadAllAssets();
        void UnloadUnusedAssets();

        // Statistics
        size_t GetLoadedAssetCount() const { return m_loadedAssetCount; }
        size_t GetTotalMemoryUsage() const { return m_totalMemoryUsage; }
        size_t GetAssetCount() const { return m_assets.size(); }

        // Utilities
        bool IsAssetLoaded(const std::string& filepath) const;
        std::vector<std::string> GetLoadedAssetPaths() const;

    private:
        void LoaderThreadFunction();
        void ProcessLoadJob(std::unique_ptr<AssetLoadJob> job);
    };

    // Concrete asset types
    class TextureAsset : public Asset {
    private:
        ComPtr<ID3D11Texture2D> m_texture;
        ComPtr<ID3D11ShaderResourceView> m_srv;
        uint32_t m_width = 0;
        uint32_t m_height = 0;
        uint32_t m_channels = 0;

    public:
        bool Load(const std::string& filepath) override;
        void Unload() override;
        size_t GetMemoryUsage() const override;

        ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }
        ID3D11Texture2D* GetTexture() const { return m_texture.Get(); }
        uint32_t GetWidth() const { return m_width; }
        uint32_t GetHeight() const { return m_height; }
        uint32_t GetChannels() const { return m_channels; }
    };

    class MeshAsset : public Asset {
    private:
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
        uint32_t m_vertexCount = 0;
        uint32_t m_indexCount = 0;
        uint32_t m_vertexStride = 0;

    public:
        bool Load(const std::string& filepath) override;
        void Unload() override;
        size_t GetMemoryUsage() const override;

        ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer.Get(); }
        ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer.Get(); }
        uint32_t GetVertexCount() const { return m_vertexCount; }
        uint32_t GetIndexCount() const { return m_indexCount; }
        uint32_t GetVertexStride() const { return m_vertexStride; }
    };
}