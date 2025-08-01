#pragma once
#include "../Core/framework.h"
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <memory>

namespace SparkEngine {
    struct ProfileSample {
        std::string name;
        double startTime;
        double endTime;
        double duration;
        uint32_t callCount;
        uint32_t threadId;
    };

    class ProfilerBlock {
    public:
        ProfilerBlock(const std::string& name);
        ~ProfilerBlock();

    private:
        std::string m_name;
        std::chrono::high_resolution_clock::time_point m_startTime;
    };

    class Profiler {
    public:
        static Profiler& Get();

        void BeginSample(const std::string& name);
        void EndSample(const std::string& name);

        void BeginFrame();
        void EndFrame();

        // Get profiling results
        const std::vector<ProfileSample>& GetFrameSamples() const { return m_frameSamples; }
        double GetFrameTime() const { return m_frameTime; }
        double GetAverageFrameTime() const;

        // GPU profiling
        void BeginGPUSample(ID3D11DeviceContext* context, const std::string& name);
        void EndGPUSample(ID3D11DeviceContext* context, const std::string& name);

        // Settings
        void SetEnabled(bool enabled) { m_enabled = enabled; }
        bool IsEnabled() const { return m_enabled; }
        void SetMaxFrameHistory(uint32_t maxFrames) { m_maxFrameHistory = maxFrames; }

        // Reset statistics
        void Reset();

    private:
        Profiler() = default;
        ~Profiler() = default;

        struct SampleData {
            std::chrono::high_resolution_clock::time_point startTime;
            uint32_t callCount = 0;
            double totalTime = 0.0;
        };

        struct GPUSample {
            ComPtr<ID3D11Query> startQuery;
            ComPtr<ID3D11Query> endQuery;
            ComPtr<ID3D11Query> disjointQuery;
            std::string name;
            bool active = false;
        };

        std::unordered_map<std::string, SampleData> m_samples;
        std::vector<ProfileSample> m_frameSamples;
        std::vector<double> m_frameTimes;
        
        std::unordered_map<std::string, std::unique_ptr<GPUSample>> m_gpuSamples;
        
        std::mutex m_mutex;
        bool m_enabled = true;
        uint32_t m_maxFrameHistory = 60;
        double m_frameTime = 0.0;
        
        std::chrono::high_resolution_clock::time_point m_frameStartTime;

        void ProcessGPUQueries(ID3D11DeviceContext* context);
    };

    // Macro for easy profiling
    #define PROFILE_SCOPE(name) ProfilerBlock _prof(name)
    #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
}
