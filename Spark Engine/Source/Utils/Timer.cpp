// Timer.cpp
#include "Timer.h"

Timer::Timer()
    : m_deltaTime(0.0f)
    , m_totalTime(0.0f)
    , m_paused(false)
{
    m_lastTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() = default;

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
    ASSERT_MSG(m_deltaTime >= 0.0f, "Delta time should never be negative");
    return m_deltaTime;
}

void Timer::UpdateTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = currentTime - m_lastTime;

    ASSERT_MSG(diff.count() >= 0.0f, "Time difference must be non-negative");
    m_deltaTime = diff.count();

    // Cap delta time to prevent large jumps
    if (m_deltaTime > 0.05f)
    {
        m_deltaTime = 0.05f;
    }

    m_lastTime += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(diff);
    m_totalTime += m_deltaTime;

    ASSERT_MSG(m_totalTime >= 0.0f, "Total time should never be negative");
}