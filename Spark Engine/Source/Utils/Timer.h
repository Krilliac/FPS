#pragma once
#include "framework.h"

namespace SparkEngine {
    class Timer {
    public:
        Timer();
        
        void Reset();
        float GetDeltaTime();
        float GetTotalTime() const;
        
        static float GetSystemTime();
        
    private:
        std::chrono::high_resolution_clock::time_point m_startTime;
        std::chrono::high_resolution_clock::time_point m_lastTime;
        float m_totalTime;
    };
}
