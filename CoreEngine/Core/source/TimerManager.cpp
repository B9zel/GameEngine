
#include <Core/includes/TimerManager.h>


unsigned long long TimerManager::timerID = 0;



void TimerManager::Update(float deltaTime)
{
    for (auto& el : m_activeTimers)
    {
        Timer& timer = el.second;
        timer.m_currentTime += deltaTime;
      
        if (timer.m_currentTime >= timer.m_fullTime)
        {
            timer.m_callback();
            if (timer.m_isLoop)
            {
                timer.m_currentTime = 0.f;
            }
            else
            {
                m_activeTimers.erase(el.first);
                break; // !!!!! CAP
            }
        }
    }
}


void TimerManager::RemoveTimer(TimerHandle& handler)
{
    if (handler.IsValid())
    {
        m_activeTimers.erase(handler.m_Hadle);
        handler.m_Hadle = 0;
    }
}

void TimerManager::PouseTimer(TimerHandle& handler)
{
    if (handler.IsValid())
    {
        if (m_activeTimers.count(handler.m_Hadle))
        {
            m_timers[handler.m_Hadle] = std::move(m_activeTimers[handler.m_Hadle]);
            m_activeTimers.erase(handler.m_Hadle);
        }
    }
}

void TimerManager::PlayTimer(TimerHandle& handler)
{
    if (handler.IsValid())
    {
        if (m_timers.count(handler.m_Hadle))
        {
            m_activeTimers[handler.m_Hadle] = std::move(m_timers[handler.m_Hadle]);
            m_timers.erase(handler.m_Hadle);
        }
    }
}
