#include "AI/WorkGoal.h"

#include "AI/Goal_Think.h"
#include "HumanStates.h"
#include "Profession.h"
#include "SHumanComponent.h"
#include "misc/ServerEnums.h"

#include <Patterns/Singleton.h>

//////////////////////////////////////////////////////////////////////////

WorkGoal::WorkGoal(SHumanComponent* pOwner)
	: Goal<SHumanComponent>(pOwner, GT_Work), m_is_going_to_work(true)
{}

WorkGoal::~WorkGoal()
{}

void WorkGoal::Activate()
{
	m_gStatus = active;
}

int WorkGoal::Process()
{
  Activate();

  if(m_is_going_to_work && !m_pOwner->GetFSM()->IsInState(Singleton<HumanMove>::GetInstancePtr()))
  {
    m_pOwner->GetFSM()->ChangeState(Singleton<HumanWork>::GetInstancePtr());
    m_is_going_to_work = false;
  }
	return m_gStatus;
}

void WorkGoal::Terminate()
{

}

bool WorkGoal::HandleMessage(const Telegram& msg)
{
	switch(msg.Msg)
	{
	case Msg_PeriodEnds:
		{
			m_gStatus = completed;
			m_pOwner->GetCurrentProfession()->HandleMessage(msg);
      m_pOwner->GetFSM()->ChangeState(Singleton<HumanIdle>::GetInstancePtr());
			return true;
		}
	}
	return false;
}