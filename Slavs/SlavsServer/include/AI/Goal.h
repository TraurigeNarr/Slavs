#ifndef Goal_h
#define Goal_h

#include "AI/GoalTypes.h"

class Telegram;

template<typename entity_type>
class Goal
{
public:
	enum GoalStatus { active, inactive, completed, failed, waitnig };
public:
							Goal(entity_type* pOwner, GoalType gType)
								: m_pOwner(pOwner), m_gType(gType), m_gStatus(inactive)
							{}

	virtual					~Goal(){}

	//logic to run when the goal is activated.
	virtual void			Activate() = 0;
	//logic to run each update-step
	virtual int				Process() = 0;
	//logic to run when the goal is satisfied. (typically used to switch
	//off any active steering behaviors)
	virtual void			Terminate() = 0;
	
	virtual bool			HandleMessage(const Telegram& msg){return false;}

	GoalType				GetType() const { return m_gType; }
	entity_type*			GetOwner() const { return m_pOwner; }

	//a Goal is atomic and cannot aggregate subgoals yet we must implement
	//this method to provide the uniform interface required for the goal
	//hierarchy.
	virtual void			AddSubgoal(Goal<entity_type>* g)
	{throw std::runtime_error("Cannot add goals to atomic goals");}

	bool					isComplete()const{return m_gStatus == completed;} 
	bool					isActive()const{return m_gStatus == active;}
	bool					isInactive()const{return m_gStatus == inactive;}
	bool					hasFailed()const{return m_gStatus == failed;}
  bool          isWaiting()const{return m_gStatus == waitnig; }
protected:
	//if m_iStatus = inactive this method sets it to active and calls Activate()
	void ActivateIfInactive();

	//if m_iStatus is failed this method sets it to inactive so that the goal
	//will be reactivated (and therefore re-planned) on the next update-step.
	void ReactivateIfFailed();

	GoalType				m_gType;
	entity_type*			m_pOwner;
	GoalStatus				m_gStatus;
};

//if m_iStatus is failed this method sets it to inactive so that the goal
//will be reactivated (replanned) on the next update-step.
template <class entity_type>
void  Goal<entity_type>::ReactivateIfFailed()
{
	if (hasFailed())
	{
		m_gStatus = inactive;
	}
}


template <class entity_type>
void  Goal<entity_type>::ActivateIfInactive()
{
	if (isInactive())
	{
		Activate();
	}
}

#endif