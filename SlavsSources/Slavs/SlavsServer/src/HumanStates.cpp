#include "HumanStates.h"
//server
#include "SGameObject.h"
#include "misc\ServerEnums.h"
//ai
#include "AI/Goal_Think.h"
//components
#include "SDynamicObjCom.h"
#include "SHumanComponent.h"
#include "SteeringBehaviors.h"
//common
#include <Patterns/Singleton.h>

//constants
const size_t DecreaseEnergyPeriod = 100;
/************************************************************************/
/*                        Human Global State                            */
/************************************************************************/
HumanGlobal::HumanGlobal()
{}

HumanGlobal::~HumanGlobal()
{}

void HumanGlobal::Enter(SHumanComponent* ip_owner)
{
}

void HumanGlobal::Execute(SHumanComponent* ip_owner, long i_elapse_time)
{
}

void HumanGlobal::Exit(SHumanComponent* ip_owner)
{
}

bool HumanGlobal::OnMessage(SHumanComponent* ip_owner, const Telegram& i_message)
{
	return false;
}
/************************************************************************/
/*                        Human Idle State                             */
/************************************************************************/
HumanIdle::HumanIdle()
{}

HumanIdle::~HumanIdle()
{}

void HumanIdle::Enter(SHumanComponent* ip_owner)
{
}

void HumanIdle::Execute(SHumanComponent* ip_owner, long i_elapsed_time)
{

}

void HumanIdle::Exit(SHumanComponent* ip_owner)
{
}

bool HumanIdle::OnMessage(SHumanComponent* ip_owner, const Telegram& i_message)
{
	switch(i_message.Msg)
	{
	case Cmd_Move:
		{
			ip_owner->GetDynamicComponent()->SetTarget(*static_cast<Vector2D*>(i_message.ExtraInfo));
			ip_owner->GetFSM()->ChangeState(Singleton<HumanMove>::GetInstancePtr());
			ip_owner->GetDynamicComponent()->GetSteering()->SeekOn();
			return true;
			break;
		}
	case Cmd_Attack:
		{
			//if(msg.Receiver == msg.Sender)//can not attack himself
			return true;
			break;
		}
	}
	return false;
}

/************************************************************************/
/*                        Human Move State                              */
/************************************************************************/
HumanMove::HumanMove()
{}

HumanMove::~HumanMove()
{}

void HumanMove::Enter(SHumanComponent* ip_owner)
{
}

void HumanMove::Execute(SHumanComponent* ip_owner, long i_elapsed_time)
{
	if(ip_owner->GetDynamicComponent()->GetPosition().Distance(ip_owner->GetDynamicComponent()->GetTarget()) < 100)
	{
		ip_owner->GetFSM()->RevertToPreviousState();
	}
}

void HumanMove::Exit(SHumanComponent* ip_owner)
{
	ip_owner->GetDynamicComponent()->SetTarget(Vector2D(0,0));
	ip_owner->GetDynamicComponent()->GetSteering()->SeekOff();
}

bool HumanMove::OnMessage(SHumanComponent* ip_owner, const Telegram& i_message)
{
	switch(i_message.Msg)
	{
	case Cmd_Move:
		{
			ip_owner->GetDynamicComponent()->SetTarget(*static_cast<Vector2D*>(i_message.ExtraInfo));
			return true;
			break;
		}
	}
	return false;
}

/************************************************************************/
/*                        Human Work State                              */
/************************************************************************/
HumanWork::HumanWork()
{}

HumanWork::~HumanWork()
{}

void HumanWork::Enter(SHumanComponent* ip_owner)
{
}

void HumanWork::Execute(SHumanComponent* ip_owner, long i_elapsed_time)
{
}

void HumanWork::Exit(SHumanComponent* ip_owner)
{
}

bool HumanWork::OnMessage(SHumanComponent* ip_owner, const Telegram& i_message)
{
	return false;
}