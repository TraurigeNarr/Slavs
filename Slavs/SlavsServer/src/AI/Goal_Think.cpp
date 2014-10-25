#include "AI/Goal_Think.h"
//human states
#include "HumanStates.h"
//evaluators
#include "AI/FindWorkGoal_Evaluator.h"
#include "AI/GetEatGoal_Evaluator.h"
#include "AI/WaitGoal_Evaluator.h"
//goals
#include "AI/FindWorkGoal.h"
#include "AI/GetEatGoal.h"
#include "AI/WaitGoal.h"
#include "AI/WorkGoal.h"
//server
#include "misc/ServerEnums.h"
//common
#include <Patterns/Telegram.h>
#include <Utilities/TemplateFunctions.h>
//standard
#include <cassert>

Goal_Think::Goal_Think(SHumanComponent* pOwner)
	: CompositeGoal<SHumanComponent>(pOwner, GT_Think)
{
	m_vEvaluators.push_back(new GetEatGoal_Evaluator(1.0f));
	m_vEvaluators.push_back(new FindWorkGoal_Evaluator(1.0f));
	m_vEvaluators.push_back(new WaitGoal_Evaluator(0.5f));
}

Goal_Think::~Goal_Think()
{
	ClearVector(m_vEvaluators);
}

void Goal_Think::Activate()
{
	Arbitrate();

	m_gStatus = active;
}

int Goal_Think::Process()
{
	int SubgoalStatus = ProcessSubgoals();

	if(SubgoalStatus == completed || SubgoalStatus == failed)
	{
		Activate();
	}
  else
    m_gStatus = active;
  ActivateIfInactive();
	return m_gStatus; 
}

void Goal_Think::Terminate()
{

}

bool Goal_Think::HandleMessage(const Telegram& msg)
{
	bool processed = ForwardMessageToFrontMostSubgoal(msg);

	if(!processed)
	{
		switch(msg.Msg)
		{
		case Msg_HumanHungry:
			{
        m_lSubGoals.push_back(new GetEatGoal(m_pOwner));
				processed = true;
				break;
			}
		case Msg_PeriodBegins:
			{
				RemoveAllSubgoals();
				m_lSubGoals.push_front(new WorkGoal(m_pOwner));
				processed = true;
				break;
			}
		}//end switch
	}

	return processed;
}

void Goal_Think::Arbitrate()
{
	Goal_Evaluator* MostDesirable = NULL;
	float best = 0.0f;
	
	for(size_t i = 0; i < m_vEvaluators.size(); ++i)
	{
		float desirability = m_vEvaluators[i]->CalculateDesirability(m_pOwner);

		if(desirability >= best)
		{
			best = desirability;
			MostDesirable = m_vEvaluators[i];
		}
	}

	assert(MostDesirable && "<Goal_Think::Arbitrate>: no target strategy selected");

	MostDesirable->SetGoal(m_pOwner);
}

bool Goal_Think::NotPresent(GoalType gType) const
{
	if (!m_lSubGoals.empty())
	{
		return m_lSubGoals.front()->GetType() != gType;
	}

	return true;
}

/*
-------------------------------------------------------------------------------------
								Add goals method
-------------------------------------------------------------------------------------
*/
void Goal_Think::AddGoal_GetEat()
{
	if(NotPresent(GT_Eat))
	{
    RemoveAllSubgoals();
		AddSubgoal(new GetEatGoal(m_pOwner));
	}
}

void Goal_Think::AddGoal_FindWork()
{
	if(NotPresent(GT_FindWork))
	{
		AddSubgoal(new FindWorkGoal(m_pOwner));
	}
}

void Goal_Think::AddGoal_Wait()
{
	if(NotPresent(GT_Wait))
	{
		RemoveAllSubgoals();
		AddSubgoal(new WaitGoal(m_pOwner));
	}
}

void Goal_Think::AddGoal_MoveToPosition(Vector2D pos)
{

}

void Goal_Think::QueueGoal_MoveToPosition(Vector2D pos)
{

}