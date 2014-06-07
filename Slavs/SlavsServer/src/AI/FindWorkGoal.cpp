#include "AI/FindWorkGoal.h"

#include "SResourceManager.h"
#include "SGameObject.h"
#include "IController.h"

#include "Management/Goverment.h"
#include "Management/IEconomyManager.h"
//components
#include "SHumanComponent.h"
#include "Profession.h"
#include "SManufacureCom.h"
//server
#include "ServerUtils.h"
#include "misc/ServerEnums.h"

#include <cassert>

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
  Slavs::TEmployersInformation informations;
  m_pOwner->GetOwner()->GetOwner()->GetGoverment().GetEconomyManager()->GetAvailableEmployers(informations);
	ParseRequests(informations);
}

int FindWorkGoal::Process()
{
	ActivateIfInactive();
	return m_gStatus;
}

void FindWorkGoal::Terminate()
{

}

void FindWorkGoal::ParseRequests(const Slavs::TEmployersInformation& i_informations)
{
	if(i_informations.empty())
	{
		m_gStatus = waitnig;
		return;
	}
  throw std::runtime_error("Deprecated usage");
	/*Slavs::TEmployersInformation::const_iterator reqIter = i_informations.begin();

	const HumanProfessions& professions = m_pOwner->GetProfessions();
	HumanProfessions::const_iterator profIter;
	//desirability on skill
	float fDskill = 0.0f;
	//desirability on payment
	float fDpayment = 0.0f;
	//total desirability of place of work
	float fTotalDes = 0.0f;
	float fBestDesirability = 0.0f;
	const EmployerInformation* BestRequest = nullptr;
	//choose most desirable place of work
  for ( ; reqIter != i_informations.end(); ++reqIter)
    {
    const EmployerInformation* p_information = *reqIter;
    profIter = professions.find(p_information->GetRequest().otNeededType);
    //if this request is not available now or human`s skill does not suitable for manufacture
    assert ((*reqIter)->IsActive());
    //if human has this profession
    if(professions.end() != profIter)
      {
      fDskill = m_fInterestingJobCoef * (p_information->GetRequest().NeededSkill()/p_information->GetOwner()->GetMaxSkill());
      fDpayment = m_fLookForPaymentCoef * p_information->GetRequest().RelativePayment();
      fTotalDes = fDskill + fDpayment;
      }
    //if human has not this profession
    else
      {
      float temp = p_information->GetRequest().NeededSkill()/p_information->GetOwner()->GetMaxSkill();
      fDskill = m_fInterestingJobCoef * (p_information->GetRequest().NeededSkill()/p_information->GetOwner()->GetMaxSkill());
      fDpayment = m_fLookForPaymentCoef * p_information->GetRequest().RelativePayment();
      fTotalDes = fDskill + fDpayment;
      }
    if(fBestDesirability <= fTotalDes)
      {
      fBestDesirability = fTotalDes;
      BestRequest = p_information;
      }
    }
	if(nullptr != BestRequest)
	  {
		m_gStatus = completed;
    BestRequest->GetOwner()->HireWorker(m_pOwner);
	  }
	else
	  {
		m_gStatus = waitnig;
	  }*/
}

bool FindWorkGoal::HandleMessage(const Telegram& msg)
{
	switch(msg.Msg)
	{
	case Msg_NewRequestsAvailable:
		{
    Slavs::TEmployersInformation informations;
    m_pOwner->GetOwner()->GetOwner()->GetGoverment().GetEconomyManager()->GetAvailableEmployers(informations);
    ParseRequests(informations);
		return true;
		}
	}
	return false;
}