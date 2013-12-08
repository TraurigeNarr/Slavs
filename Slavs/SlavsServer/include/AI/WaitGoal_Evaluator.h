#ifndef WaitGoal_Evaluator_h
#define WaitGoal_Evaluator_h

#include "AI/Goal_Evaluator.h"

class WaitGoal_Evaluator : public Goal_Evaluator
{
public:
			WaitGoal_Evaluator(float fHumanBias);
			~WaitGoal_Evaluator();

	float	CalculateDesirability(SHumanComponent* pHuman);
	void	SetGoal(SHumanComponent* pHuman);


};

#endif