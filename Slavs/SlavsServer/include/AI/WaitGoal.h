#ifndef WaitGoal_h
#define WaitGoal_h

#include "AI/Goal.h"
#include "SHumanComponent.h"

class WaitGoal : public Goal<SHumanComponent>
{
public:
	WaitGoal(SHumanComponent* pOwner);
	~WaitGoal();

	void	Activate();
	int		Process();
	void	Terminate();

	bool	HandleMessage(const Telegram& msg);
protected:
	size_t m_uiTime;
};

#endif