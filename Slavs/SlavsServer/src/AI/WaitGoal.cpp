#include "AI/WaitGoal.h"
#include "misc/ServerEnums.h"

WaitGoal::WaitGoal(SHumanComponent* pOwner)
	: Goal<SHumanComponent>(pOwner, GT_Wait), m_uiTime(0)
{}

WaitGoal::~WaitGoal()
{}

void WaitGoal::Activate()
{
	m_gStatus = active;
}

int WaitGoal::Process()
{
	ActivateIfInactive();
	return m_gStatus;
}

void WaitGoal::Terminate()
{

}

bool WaitGoal::HandleMessage(const Telegram& msg)
{
	switch(msg.Msg)
	{
	case Msg_NewRequestsAvailable:
		{
			m_gStatus = completed;
			return true;
			break;
		}
	}
	return false;
}