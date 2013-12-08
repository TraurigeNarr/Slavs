#include "AI/GetEatGoal_Evaluator.h"
#include "SHumanComponent.h"
#include "ServerUtils.h"

#include "AI/Goal_Think.h"

GetEatGoal_Evaluator::GetEatGoal_Evaluator(float fHumanBias)
	: Goal_Evaluator(fHumanBias)
{
  Clamp(m_fHumanBias, 0.5f, 0.8f);
}

GetEatGoal_Evaluator::~GetEatGoal_Evaluator()
{}

float GetEatGoal_Evaluator::CalculateDesirability(SHumanComponent* pHuman)
{
  float ratio = 0.0f;
  if(pHuman->LeftToLifeRatio(ratio))
    return ratio > m_fHumanBias ? 1.0f : ratio;
  else
    return 0.0f;
}

void GetEatGoal_Evaluator::SetGoal(SHumanComponent* pHuman)
{
	pHuman->GetBrain()->AddGoal_GetEat();
}