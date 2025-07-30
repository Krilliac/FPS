// Timer.h
#pragma once

#include "..\Core\framework.h"
#include "Utils/Assert.h"
#include <chrono>

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point m_lastTime;
    float                                          m_deltaTime;
    float                                          m_totalTime;
    bool                                           m_paused;

public:
    Timer();
    ~Timer();

    void  Start();
    void  Stop();
    void  Reset();
    float GetDeltaTime();
    float GetTotalTime() const { return m_totalTime; }

    bool IsPaused() const { return m_paused; }

private:
    void UpdateTime();
};