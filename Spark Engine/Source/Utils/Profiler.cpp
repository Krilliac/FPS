#include "Profiler.h"
#include <algorithm>
#include <iostream>

namespace SparkEngine {
    ProfilerBlock::ProfilerBlock(const std::string& name) : m_name(name) {
        m_startTime = std::chrono::high_resolution_clock::now();
        Profiler::Get().BeginSample(name);
    }

    ProfilerBlock::~ProfilerBlock() {
        Profiler::Get().EndSample(m_name);
    }

    Profiler& Profiler::Get() {
        static Profiler instance;
        return instance;
    }

    void Profiler::BeginSample(const std::string& name) {
        if (!m_enabled) return;

        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto& sample = m_samples[name];
        sample.startTime = std::chrono::high_resolution_clock::now();
        sample.callCount++;
    }

    void Profiler::EndSample(const std::string& name) {
        if (!m_enabled) return;

        auto endTime = std::chrono::high_resolution_clock::now();
        
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto it = m_samples.find(name);
        if (it != m_samples.end()) {
            auto duration = std::chrono::duration<double>(endTime - it->second.startTime).count();
            it->second.totalTime += duration;
        }
    }

    void Profiler::BeginFrame() {
        if (!m_enabled) return;
        
        m_frameStartTime = std::chrono::high_resolution_clock::now();
    }

    void Profiler::EndFrame() {
        if (!m_enabled) return;

        auto frameEndTime = std::chrono::high_resolution_clock::now();
        m_frameTime = std::chrono::duration<double>(frameEndTime - m_frameStartTime).count();
        
        // Store frame time for averaging
        m_frameTimes.push_back(m_frameTime);
        if (m_frameTimes.size() > m_maxFrameHistory) {
            m_frameTimes.erase(m_frameTimes.begin());
        }

        // Create frame samples
        std::lock_guard<std::mutex> lock(m_mutex);
        m_frameSamples.clear();
        
        for (const auto& [name, data] : m_samples) {
            if (data.callCount > 0) {
                ProfileSample sample;
                sample.name = name;
                sample.duration = data.totalTime / data.callCount;
                sample.callCount = data.callCount;
                sample.threadId = GetCurrentThreadId();
                m_frameSamples.push_back(sample);
            }
        }

        // Reset samples for next frame
        for (auto& [name, data] : m_samples) {
            data.callCount = 0;
            data.totalTime = 0.0;
        }
    }

    double Profiler::GetAverageFrameTime() const {
        if (m_frameTimes.empty()) return 0.0;
        
        double sum = 0.0;
        for (double time : m_frameTimes) {
            sum += time;
        }
        return sum / m_frameTimes.size();
    }

    void Profiler::Reset() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_samples.clear();
        m_frameSamples.clear();
        m_frameTimes.clear();
        m_frameTime = 0.0;
    }
}
