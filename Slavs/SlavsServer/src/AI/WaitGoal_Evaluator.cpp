#include "AI/WaitGoal_Evaluator.h"
#include "SHumanComponent.h"
#include "ServerUtils.h"

#include "AI/Goal_Think.h"

WaitGoal_Evaluator::WaitGoal_Evaluator(float fHumanBias)
	: Goal_Evaluator(fHumanBias)
{}

WaitGoal_Evaluator::~WaitGoal_Evaluator()
{}

float WaitGoal_Evaluator::CalculateDesirability(SHumanComponent* pHuman)
{
	//return constant value because human will wait only if there are nothing to do
	return 0.1f*m_fHumanBias;
}

void WaitGoal_Evaluator::SetGoal(SHumanComponent* pHuman)
{
	pHuman->GetBrain()->AddGoal_Wait();
}

