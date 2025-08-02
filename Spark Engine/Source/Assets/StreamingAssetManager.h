#pragma once
#include "../Core/framework.h"
#include "../Assets/AssetManager.h"
#include "../Networking/NetworkCore.h"
#include <unordered_set>
#include <thread>
#include <atomic>

namespace SparkEngine {
    struct StreamingRegion {
        XMFLOAT3 center;
        float radius;
        uint32_t priority;
        std::unordered_set<std::string> requiredAssets;
    };

    enum class StreamingState {
        NotLoaded, Requested, Downloading, Loaded, Failed
    };

    struct AssetStreamInfo {
        std::string assetId;
        StreamingState state = StreamingState::NotLoaded;
        float priority = 0.0f;
        size_t dataSize = 0;
        uint64_t lastAccessed = 0;
        std::vector<uint8_t> compressedData;
    };

    class StreamingAssetManager {
    private:
        AssetManager* m_assetManager;
        NetworkCore* m_networkCore;
        
        // Streaming state
        std::unordered_map<std::string, AssetStreamInfo> m_streamingAssets;
        std::vector<StreamingRegion> m_streamingRegions;
        std::unordered_set<std::string> m_activeAssets;
        
        // Streaming parameters
        size_t m_maxMemoryUsage = 512 * 1024 * 1024; // 512MB
        size_t m_currentMemoryUsage = 0;
        float m_streamingDistance = 300.0f;
        uint32_t m_maxConcurrentDownloads = 8;
        
        // Threading
        std::vector<std::thread> m_streamingThreads;
        std::queue<std::string> m_downloadQueue;
        std::mutex m_queueMutex;
        std::condition_variable m_queueCondition;
        std::atomic<bool> m_isRunning{false};

    public:
        StreamingAssetManager(AssetManager* assetManager, NetworkCore* networkCore);
        ~StreamingAssetManager();

        bool Initialize();
        void Shutdown();
        void Update();

        // Region-based streaming
        void AddStreamingRegion(const XMFLOAT3& center, float radius, uint32_t priority = 1);
        void RemoveStreamingRegion(const XMFLOAT3& center);
        void UpdateStreamingRegions(const XMFLOAT3& viewerPosition);
        
        // Asset streaming control
        void RequestAssetStream(const std::string& assetId, float priority = 1.0f);
        void CancelAssetStream(const std::string& assetId);
        bool IsAssetStreamed(const std::string& assetId) const;
        StreamingState GetAssetStreamingState(const std::string& assetId) const;
        
        // Memory management
        void SetMaxMemoryUsage(size_t maxBytes) { m_maxMemoryUsage = maxBytes; }
        size_t GetCurrentMemoryUsage() const { return m_currentMemoryUsage; }
        void FreeUnusedAssets();
        void EvictLeastRecentlyUsed();
        
        // Compression and optimization
        std::vector<uint8_t> CompressAssetData(const std::vector<uint8_t>& data);
        std::vector<uint8_t> DecompressAssetData(const std::vector<uint8_t>& compressedData);
        
        // Predictive streaming
        void PredictRequiredAssets(const XMFLOAT3& position, const XMFLOAT3& velocity);
        void PreloadAssetsForArea(const XMFLOAT3& center, float radius);
        
        // Statistics and monitoring
        float GetStreamingProgress() const;
        size_t GetActiveDownloadCount() const;
        std::vector<std::string> GetMostRecentlyUsedAssets(size_t count) const;

    private:
        void StreamingThreadFunction();
        void ProcessDownloadQueue();
        bool DownloadAsset(const std::string& assetId);
        void HandleAssetReceived(const NetworkPacket& packet);
        float CalculateAssetPriority(const std::string& assetId, const XMFLOAT3& viewerPos);
        bool ShouldEvictAsset(const AssetStreamInfo& assetInfo) const;
    };
}

