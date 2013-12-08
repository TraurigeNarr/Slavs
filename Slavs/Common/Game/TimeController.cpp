#include "TimeController.h"

#include <algorithm>
#include <cassert>

TimeController::TimeController()
  : m_lMsForTick(0), m_lMsPast(0)
{

}

TimeController::TimeController(long msForTick)
	: m_lMsForTick(msForTick), m_lMsPast(0)
{}

TimeController::~TimeController()
{
  m_vListeners.clear();
}

void TimeController::Update(long elapsedTime)
{
	m_lMsPast += elapsedTime;
	if(m_lMsForTick > m_lMsPast)
		return;
	std::for_each(m_vListeners.begin(), m_vListeners.end(), [](TickListener* listener)
	{
		listener->TickPerformed();
	});
	m_lMsPast = 0;
}

void TimeController::AddSubscriber(TickListener* listener)
{
	assert(listener && "<STimeController::AddSubscriber>:listener is NULL");
	m_vListeners.push_back(listener);
}

void TimeController::RemoveSubscriber(TickListener* listener)
{
	for(size_t i = 0; i < m_vListeners.size(); ++i)
	{
		if(m_vListeners.at(i) == listener)
		{
			m_vListeners.erase(m_vListeners.begin() + i);
			return;
		}
	}
}