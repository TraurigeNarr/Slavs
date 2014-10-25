#ifndef CompositeGoal_h
#define CompositeGoal_h

#include "AI/Goal.h"

#include <list>

template <typename entity_type>
class CompositeGoal : public Goal<entity_type>
{
	typedef std::list< Goal<entity_type>* > SubgoalList;

public:
	CompositeGoal(entity_type* pOwner, GoalType gType) : Goal<entity_type>(pOwner, gType) {}
	virtual ~CompositeGoal(){RemoveAllSubgoals();}

	virtual void			Activate() = 0;
	virtual int				Process() = 0;
	virtual void			Terminate() = 0;

	void					AddSubgoal(Goal<entity_type>* pGoal);
	void					RemoveAllSubgoals();
protected:
	//processes any subgoals that may be present
	int						ProcessSubgoals();
	//passes the message to the front subgoal
	bool					ForwardMessageToFrontMostSubgoal(const Telegram& msg);

	SubgoalList				m_lSubGoals;
};


//---------------------- RemoveAllSubgoals ------------------------------------
//-----------------------------------------------------------------------------
template <class entity_type>
void CompositeGoal<entity_type>::RemoveAllSubgoals()
{
	while(!m_lSubGoals.empty())
	{
		m_lSubGoals.front()->Terminate();
		delete m_lSubGoals.front();
		m_lSubGoals.pop_front();
	}
}

//-------------------------- ProcessSubGoals ----------------------------------
//
//  this method first removes any completed goals from the front of the
//  subgoal list. It then processes the next goal in the list (if there is one)
//-----------------------------------------------------------------------------
template <class entity_type>
int CompositeGoal<entity_type>::ProcessSubgoals()
{ 
	//remove all completed and failed goals from the front of the subgoal list
	while (!m_lSubGoals.empty() &&
		(m_lSubGoals.front()->isComplete() || m_lSubGoals.front()->hasFailed()))
	{    
		m_lSubGoals.front()->Terminate();
		delete m_lSubGoals.front(); 
		m_lSubGoals.pop_front();
	}

	//if any subgoals remain, process the one at the front of the list
	if (!m_lSubGoals.empty())
	{ 
		//grab the status of the front-most subgoal
		int StatusOfSubGoals = m_lSubGoals.front()->Process();

		//we have to test for the special case where the front-most subgoal
		//reports 'completed' *and* the subgoal list contains additional goals.When
		//this is the case, to ensure the parent keeps processing its subgoal list
		//we must return the 'active' status.
		if (StatusOfSubGoals == completed && m_lSubGoals.size() > 1)
		{
      m_lSubGoals.pop_front();
			return active;
		}

    if(StatusOfSubGoals == waitnig)
    {
      //TODO: do something with goals
      //supposedly current_goal will be GetEat, because FindWorkGoal waits for work, but it may be not work
      SubgoalList::iterator current_goal = m_lSubGoals.begin();
      ++current_goal;
      if(current_goal != m_lSubGoals.end())
        if(completed == (*current_goal)->Process())
        {
          (*current_goal)->Terminate();
          delete (*current_goal);
          m_lSubGoals.erase(current_goal);
        }
    }

		return StatusOfSubGoals;
	}
	//no more subgoals to process - return 'completed'
	else
	{
		return completed;
	}
}

//----------------------------- AddSubgoal ------------------------------------
template <class entity_type>
void CompositeGoal<entity_type>::AddSubgoal(Goal<entity_type>* g)
{   
	//add the new goal to the front of the list
	m_lSubGoals.push_front(g);
}



//---------------- ForwardMessageToFrontMostSubgoal ---------------------------
//
//  passes the message to the goal at the front of the queue
//-----------------------------------------------------------------------------
template <class entity_type>
bool CompositeGoal<entity_type>::ForwardMessageToFrontMostSubgoal(const Telegram& msg)
{
	if (!m_lSubGoals.empty())
	{
		return m_lSubGoals.front()->HandleMessage(msg);
	}

	//return false if the message has not been handled
	return false;
}

#endif