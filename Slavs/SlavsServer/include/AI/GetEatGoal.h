#ifndef GetEatGoal_h
#define GetEatGoal_h

#include "AI/Goal.h"
#include "SHumanComponent.h"

class GetEatGoal : public Goal<SHumanComponent>
{
public:
	GetEatGoal(SHumanComponent* pOwner);
	~GetEatGoal();

	void	Activate();
	int		Process();
	void	Terminate();
private:
  void GetFood();
};

#endif