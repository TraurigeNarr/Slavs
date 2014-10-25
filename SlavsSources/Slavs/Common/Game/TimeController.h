#ifndef STimeController_h
#define STimeController_h

#include <vector>

class TickListener
{
public:
	virtual void TickPerformed() = 0;
};

class TimeController
{
public:
  TimeController();
	//@param msForTick - number of milliseconds for one game tick
	TimeController(long msForTick);
  ~TimeController();

	void Update(long elapsedTime);
	void AddSubscriber(TickListener* listener);
	void RemoveSubscriber(TickListener* listener);
protected:
	//list of listeners that will handle message that tick is performed
	std::vector<TickListener*> m_vListeners;

	long m_lMsForTick;
	//determines milliseconds that passed from last tick
	long m_lMsPast;
};

#endif