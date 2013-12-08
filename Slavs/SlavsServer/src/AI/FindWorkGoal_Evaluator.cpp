#include "AI/FindWorkGoal_Evaluator.h"
#include "AI/Goal_Think.h"

#include "SHumanComponent.h"

#include <Math/MathUtilities.h>

FindWorkGoal_Evaluator::FindWorkGoal_Evaluator(float fHumanBias)
	: Goal_Evaluator(fHumanBias)
{
	MathUtilities::Clamp(m_fHumanBias, 0.4f, 0.9f);
}

FindWorkGoal_Evaluator::~FindWorkGoal_Evaluator()
{}

float FindWorkGoal_Evaluator::CalculateDesirability(SHumanComponent* pHuman)
{
	//he needs work
	if(!pHuman->HasWork())
	{
    float ratio = 0.0f;
    if(pHuman->LeftToLifeRatio(ratio))
      return ratio < 0.6f ? 1.0f : m_fHumanBias;
		return m_fHumanBias;
	}
	//he has work
	return 0.0f;
}

void FindWorkGoal_Evaluator::SetGoal(SHumanComponent* pHuman)
{
	pHuman->GetBrain()->AddGoal_FindWork();
}