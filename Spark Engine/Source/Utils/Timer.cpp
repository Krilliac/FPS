#include "Timer.h"

namespace SparkEngine {
    Timer::Timer() : m_totalTime(0.0f) {
        Reset();
    }

    void Timer::Reset() {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_lastTime = m_startTime;
        m_totalTime = 0.0f;
    }

    float Timer::GetDeltaTime() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
        
        m_lastTime = currentTime;
        m_totalTime += deltaTime;
        
        // Cap delta time to prevent huge jumps
        return std::min(deltaTime, 0.033f); // Max 33ms (30 FPS)
    }

    float Timer::GetTotalTime() const {
        return m_totalTime;
    }

    float Timer::GetSystemTime() {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float>(currentTime - startTime).count();
    }
}
