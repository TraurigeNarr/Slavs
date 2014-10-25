#ifndef WorkGoal_h
#define WorkGoal_h

#include "AI/Goal.h"
#include "SHumanComponent.h"

class WorkGoal : public Goal<SHumanComponent>
{
public:
	WorkGoal(SHumanComponent* pOwner);
	~WorkGoal();

	void	Activate();
	int		Process();
	void	Terminate();

	bool	HandleMessage(const Telegram& msg);
private:
  bool m_is_going_to_work;
};

#endif