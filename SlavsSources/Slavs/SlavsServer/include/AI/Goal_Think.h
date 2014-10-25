#ifndef Goal_Think_h
#define Goal_Think_h

#include "AI/CompositeGoal.h"
#include "AI/Goal_Evaluator.h"
#include "SHumanComponent.h"
//common
#include <Math/Vector2D.h>

#include <vector>

class Goal_Think : public CompositeGoal<SHumanComponent>
{
	typedef std::vector<Goal_Evaluator*>   GoalEvaluators;
public:
					Goal_Think(SHumanComponent* pOwner);
					~Goal_Think();

	void			Arbitrate();

	void			Activate();
	int				Process();
	void			Terminate();

	bool			HandleMessage(const Telegram& msg);

	void			AddGoal_GetEat();
	void			AddGoal_FindWork();
	void			AddGoal_Wait();
	void			AddGoal_MoveToPosition(Vector2D pos);
	//this adds the MoveToPosition goal to the *back* of the subgoal list.
	void			QueueGoal_MoveToPosition(Vector2D pos);
protected:
	//returns true if parameter`s goal type is not on the top of list
	bool			NotPresent(GoalType gType) const;

	GoalEvaluators	m_vEvaluators;
};

#endif