#include "AI/FindWorkGoal.h"
#include "SResourceManager.h"
#include "SGameObject.h"
#include "IController.h"
//components
#include "SHumanComponent.h"
#include "Profession.h"
#include "SManufacureCom.h"
//server
#include "ServerUtils.h"
#include "misc/ServerEnums.h"

FindWorkGoal::FindWorkGoal(SHumanComponent* pOwner)
	: Goal<SHumanComponent>(pOwner, GT_FindWork)
{
	m_fInterestingJobCoef = RandInRange(0.3f, 1.0f);
  m_fLookForPaymentCoef = RandInRange(0.3f, 1.0f);
}

FindWorkGoal::~FindWorkGoal()
{}

void FindWorkGoal::Activate()
{
	m_gStatus = active;
	ParseRequests(m_pOwner->GetOwner()->GetOwner()->GetResourceMgr()->GetAllRequests());
}

int FindWorkGoal::Process()
{
	ActivateIfInactive();
	return m_gStatus;
}

void FindWorkGoal::Terminate()
{

}

void FindWorkGoal::ParseRequests(ManufactureRequests& requests)
{
	if(requests.empty())
	{
		m_gStatus = waitnig;
		return;
	}
	ManufactureRequests::iterator reqIter = requests.begin();

	const HumanProfessions& professions = m_pOwner->GetProfessions();
	HumanProfessions::const_iterator profIter;
	//desirability on skill
	float fDskill = 0.0f;
	//desirability on payment
	float fDpayment = 0.0f;
	//total desirability of place of work
	float fTotalDes = 0.0f;
	float fBestDesirability = 0.0f;
	ManufactureRequest* BestRequest = NULL;
	//choose most desirable place of work
  for ( ; reqIter != requests.end(); ++reqIter)
  {
		profIter = professions.find(reqIter->second->GetRequest().otNeededType);
		//if this request is not available now or human`s skill does not suitable for manufacture
		if( !reqIter->second->GetRequest().bAvailability)
			continue;
		//if human has this profession
		if(professions.end() != profIter)
		{
			fDskill = m_fInterestingJobCoef * (reqIter->second->GetRequest().fNeededSkill/reqIter->first->GetMaxSkill());
			fDpayment = m_fLookForPaymentCoef * (reqIter->second->GetRequest().uiPayment/(reqIter->second->GetRequest().uiMaxPayment*1.0f));
			fTotalDes = fDskill + fDpayment;
		}
		//if human has not this profession
		else
		{
      float temp = reqIter->second->GetRequest().fNeededSkill/reqIter->first->GetMaxSkill();
			fDskill = m_fInterestingJobCoef * (reqIter->second->GetRequest().fNeededSkill/reqIter->first->GetMaxSkill());
			fDpayment = m_fLookForPaymentCoef * (reqIter->second->GetRequest().uiPayment/(reqIter->second->GetRequest().uiMaxPayment*1.0f));
			fTotalDes = fDskill + fDpayment;
		}
		if(fBestDesirability <= fTotalDes)
		{
			fBestDesirability = fTotalDes;
			BestRequest = reqIter->second;
		}
	}
	if(NULL != BestRequest)
	{
		m_gStatus = completed;
    BestRequest->GetOwner()->HireWorker(m_pOwner);
	}
	else
	{
		m_gStatus = waitnig;
	}
}

bool FindWorkGoal::HandleMessage(const Telegram& msg)
{
	switch(msg.Msg)
	{
	case Msg_NewRequestsAvailable:
		{
			ParseRequests(m_pOwner->GetOwner()->GetOwner()->GetResourceMgr()->GetAllRequests());
			return true;
		}
	}
	return false;
}