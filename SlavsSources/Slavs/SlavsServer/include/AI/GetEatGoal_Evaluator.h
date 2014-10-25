#ifndef GetEatGoal_Evaluator_h
#define GetEatGoal_Evaluator_h

#include "AI/Goal_Evaluator.h"



class GetEatGoal_Evaluator : public Goal_Evaluator
{
public:
			GetEatGoal_Evaluator(float fHumanBias);
			~GetEatGoal_Evaluator();

	float	CalculateDesirability(SHumanComponent* pHuman);

	void	SetGoal(SHumanComponent* pHuman);
};

#endif