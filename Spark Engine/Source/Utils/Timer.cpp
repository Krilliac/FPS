#include "Timer.h"

Timer::Timer()
    : m_deltaTime(0.0f)
    , m_totalTime(0.0f)
    , m_paused(false)
{
    m_lastTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
}

void Timer::Start()
{
    if (m_paused)
    {
        m_lastTime = std::chrono::high_resolution_clock::now();
        m_paused = false;
    }
}

void Timer::Stop()
{
    m_paused = true;
}

void Timer::Reset()
{
    m_lastTime = std::chrono::high_resolution_clock::now();
    m_deltaTime = 0.0f;
    m_totalTime = 0.0f;
    m_paused = false;
}

float Timer::GetDeltaTime()
{
    if (!m_paused)
    {
        UpdateTime();
    }
    return m_deltaTime;
}

void Timer::UpdateTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
    m_lastTime = currentTime;
    m_totalTime += m_deltaTime;
    
    // Cap delta time to prevent large jumps
    if (m_deltaTime > 0.05f)
    {
        m_deltaTime = 0.05f;
    }
}

