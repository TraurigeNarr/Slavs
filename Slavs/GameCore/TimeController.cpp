#include "stdafx.h"

#include "TimeController.h"

TimeController::TimeController()
  : m_ms_for_tick(0)
  , m_ms_past(0)
  {

  }

TimeController::TimeController(long i_ms_for_tick)
  : m_ms_for_tick(i_ms_for_tick)
  , m_ms_past(0)
  {}

TimeController::~TimeController()
  {
  m_listeners.clear();
  }

void TimeController::Update(long i_elapsed_time)
  {
  m_ms_past += i_elapsed_time;
  
  if(m_ms_for_tick > m_ms_past)
    return;
  
  for (TickListener* p_listener : m_listeners)
    p_listener->TickPerformed();

  m_ms_past = 0;
  }

void TimeController::AddSubscriber(TickListener* ip_listener)
  {
  assert(ip_listener && "<STimeController::AddSubscriber>:listener is NULL");
  m_listeners.push_back(ip_listener);
  }

void TimeController::RemoveSubscriber(TickListener* ip_listener)
  {
  auto it_end = std::remove(m_listeners.begin(), m_listeners.end(), ip_listener);
  m_listeners.erase(it_end, m_listeners.end());
  }