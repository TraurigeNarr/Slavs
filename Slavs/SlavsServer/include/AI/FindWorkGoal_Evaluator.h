#ifndef FindWorkGoal_Evaluator_h
#define FindWorkGoal_Evaluator_h

#include "AI/Goal_Evaluator.h"


class FindWorkGoal_Evaluator : public Goal_Evaluator
{
public:
			FindWorkGoal_Evaluator(float fHumanBias);
			~FindWorkGoal_Evaluator();

	float	CalculateDesirability(SHumanComponent* pHuman);
	void	SetGoal(SHumanComponent* pHuman);
};

#endif