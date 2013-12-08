#include "SManufacureCom.h"
//server
#include "SBuildingComp.h"
#include "IController.h"
#include "SGameContext.h"
#include "SGameObject.h"
#include "SHumanComponent.h"
#include "SStaticObjCom.h"
#include "SStaticObjCom.h"
#include "misc\ServerEnums.h"
#include "misc\ServerEnums.h"

#include "Management/Goverment.h"
#include "Management/IEconomyManager.h"
#include "Management/IStoreSystem.h"

//common
#include <Game/GameObjectState.h>
#include <Patterns/Telegram.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <cassert>
#include <string>

SManufacureCom::SManufacureCom(SGameObject* owner, const TiXmlElement* componentElement)
	: m_pOwner(owner), m_pBoxOfResources(NULL), 
	m_iCurrentTick(0), m_bWorking(false), m_iWorkersEndsCycle(0)
{
	assert(componentElement &&
		"<SResourceMining::SResourceMining>: NULL parameter of config element.");
	m_pRequest = new ManufactureRequest(this);
	ParseElelement(componentElement);
	m_pOwner->GetOwner()->GetResourceMgr()->RegisterManufacture(this, m_pRequest);
  m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->RegisterEmployer(std::shared_ptr<SManufacureCom>(this));
}

SManufacureCom::~SManufacureCom()
{
	//send all workers notification of destroying manufacture
	for(size_t i = 0; i < m_vWorkers.size(); ++i)
	{
		m_vWorkers[i]->HandleMessage(Telegram(0,0,0, Msg_HumanFired, this));
	}
  m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->RemoveEmployer(std::shared_ptr<SManufacureCom>(this));
}

void SManufacureCom::TickPerformed()
{
	if(m_bWorking && 0 != m_vWorkers.size())
		++m_iCurrentTick;
	if(m_iOperatingCycle <= m_iCurrentTick)
  {
		m_iWorkersEndsCycle = m_iCurrentTick = 0;
		//send all workers notification of ending operating cycle
		m_pRequest->GetRequest().bAvailability = true;
		Telegram t(0,0,0, Msg_PeriodEnds, this);
		for(size_t i = 0; i < m_vWorkers.size(); ++i)
		{
			m_vWorkers[i]->HandleMessage(t);
		}
		//restore value of needed workers
		m_iNeededWorkers += m_vWorkers.size();
		//clear the list
		t.Msg = Msg_HumanFired;
		while(!m_vWorkers.empty())
		{
      if(!GetMoney(m_uiPayment))
        printf("Have no money for payment\n");
      //pay worker and release him
      m_vWorkers.back()->GiveMoney(m_uiPayment);
			m_vWorkers.back()->GetOwner()->HandleMessage(t);
			m_vWorkers.pop_back();
		}
		//if manufacture can to work on
		if(m_pRequest->GetRequest().bAvailability)
		{
			m_pOwner->GetOwner()->GetResourceMgr()->RefreshRequests();
		}
	}
}

bool SManufacureCom::HandleMessage(const Telegram& msg)
{
	bool processed = false;
	switch(msg.Msg)
	{
	case Msg_AllObjectsUpdated:
		{
			if(!m_bWorking && 0 != m_vWorkers.size())
			{
				m_bWorking = true;
				m_pRequest->GetRequest().bAvailability = false;
        for(size_t i = 0; i < m_vWorkers.size(); ++i)
        {
          //move worker to point of meeting --> temporary
          Telegram t(0, m_pOwner->GetID(), m_vWorkers[i]->GetOwner()->GetID(), Msg_PeriodBegins, this);
          m_vWorkers[i]->HandleMessage(t);
        }
				processed = true;
			}
			break;
		}
	case Msg_HumanDied:
		{
			for(size_t i = 0; i < m_vWorkers.size(); ++i)
			{
				if(m_vWorkers[i] == msg.ExtraInfo)
					m_vWorkers.erase(m_vWorkers.begin()+i);
			}
			m_pOwner->StateChanged();
			break;
		}
	case Msg_WorkerPassResources:
		{
			int *numberOfResources = (int*)msg.ExtraInfo;
			m_pBoxOfResources->Add(*numberOfResources);
			++m_iWorkersEndsCycle;
			if(m_vWorkers.size() == m_iWorkersEndsCycle)
			{
        m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Add(m_pBoxOfResources);
        /*if(m_pOwner->GetOwner()->GetResourceMgr()->Buy(m_pBoxOfResources))
        {
          GiveMoney(m_pBoxOfResources->GetNumber()*m_pBoxOfResources->Price());
          m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Add(m_pBoxOfResources);
        }*/

				m_bWorking = false;
				if(0 != m_pBoxOfResources->GetNumber())
				{
					m_pRequest->GetRequest().bAvailability = false;
          m_pRequest->GetRequest().bNeedStore = true;
					m_pTransitStore->AddResource(m_pBoxOfResources);
					m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->GetStoreSystem()->ProcessEvent(EE_NEED_STORE, this);
				}
				
				m_pBoxOfResources->Clear();
				m_iWorkersEndsCycle = 0;
			}
			m_pOwner->StateChanged();
			break;
		}
	}
	return processed;
}

void SManufacureCom::GetState(GameObjectState& goState) const
{
	goState.iFlags |= GOF_Resources;
	
	goState.vContainers.push_back(m_pTransitStore);

	goState.iFlags |= GOF_HasWorkers;
	goState.iWorkersNumber = m_vWorkers.size();

	goState.iFlags |= m_bWorking ? GOF_Working : GOF_Waiting;
    
  goState.iFlags |= GOF_RequestForWorkers;
  m_pRequest->GetRequest().fAverageSkill = 0.0f;
  for(size_t i = 0; i < m_vWorkers.size(); ++i)
    m_pRequest->GetRequest().fAverageSkill += m_vWorkers[i]->GetCurrentProfession()->GetSkill();
  m_pRequest->GetRequest().fAverageSkill /= m_vWorkers.size();
  goState.request_for_workers = new RequestForWorkers(m_pRequest->GetRequest());

  goState.iFlags |= GOF_Money;
  goState.uiMoney = HasMoney();
}

void SManufacureCom::ParseElelement(const TiXmlElement* componentElement)
{
	m_fInitialSkill				= XmlUtilities::GetRealAttribute(componentElement, "initialSkill", 0);
	m_learning_coefficient= XmlUtilities::GetRealAttribute(componentElement, "learningCoef", 0);
  m_fMaxSkill           = XmlUtilities::GetRealAttribute(componentElement, "maxSkill", 0);
	m_fSkillGrowth				= XmlUtilities::GetRealAttribute(componentElement, "skillGrowth", 0);
	m_iNeededWorkers			= XmlUtilities::GetIntAttribute(componentElement, "neededWorkers", 0);
	m_iGrowthPeriod				= XmlUtilities::GetIntAttribute(componentElement, "growthPeriod", 0);
	m_iOperatingCycle			= XmlUtilities::GetIntAttribute(componentElement, "operatingCycle", 10);
	m_iIniResMines				= XmlUtilities::GetIntAttribute(componentElement, "iniResMines", 10);
	m_uiPayment					  = XmlUtilities::GetIntAttribute(componentElement, "payment", 10);
  m_WorkerType          = IGameObject::GetGameObjectType(XmlUtilities::GetStringAttribute(componentElement, "workerType", "worker"));
  //set initial budget
  GiveMoney(XmlUtilities::GetIntAttribute(componentElement, "money", m_uiPayment*m_iNeededWorkers));
  printf("Has money:%d\n", HasMoney());
	m_grType					= GameResourceBox::DefineResType(XmlUtilities::GetStringAttribute(componentElement, "resType", ""));
  ResourceConfigs res_confs;
  res_confs.m_price = 1;
  m_pOwner->GetOwner()->GetContext()->GetResConfigcs(m_grType, res_confs);
	m_pBoxOfResources			= new GameResourceBox(m_grType, 0, 0, res_confs.m_price);
	m_pTransitStore				= std::make_shared<GameResourceContainer>(GameResourceContainer(m_grType, m_iNeededWorkers*m_iIniResMines*m_fMaxSkill));

	m_pRequest->GetRequest().bAvailability	= true;
	m_pRequest->GetRequest().fNeededSkill	= m_fInitialSkill;
	m_pRequest->GetRequest().otNeededType	= m_WorkerType;
	m_pRequest->GetRequest().grType			= m_grType;
	m_pRequest->GetRequest().uiPayment		= m_uiPayment;
	
	RequestForWorkers::uiMaxPayment		= RequestForWorkers::uiMaxPayment >= m_uiPayment ? RequestForWorkers::uiMaxPayment : m_uiPayment;
}

bool SManufacureCom::Probe()
{
	//checks for SBuilding component
	assert(m_pOwner->GetComponent<SBuildingComp>() &&
		"<SStoreHouseCom::Probe>: NULL pointer of SDynamicObjCom.");
	return true;
}

GameResourceType SManufacureCom::GetGRType() const
{
	return m_grType;
}

bool SManufacureCom::HireWorker(SHumanComponent* worker)
{
	assert(worker && "<SResourceMiningCom::HireWorker>: worker is NULL.");

	if(m_bWorking)
	{
		return false;
	}
	m_vWorkers.push_back(worker);
  
  --m_iNeededWorkers;
  if(0 == m_iNeededWorkers)
    m_pRequest->GetRequest().bAvailability = false;

	m_pOwner->StateChanged();
  //send worker some information
  worker->GetOwner()->HandleMessage(Telegram(0,0,0, Msg_ObtainProfession, this));
  Vector2D vec = (m_pOwner->GetComponent<SStaticObjCom>()->GetPosition());
  vec += Vector2D(rand()%500-500,rand()%500-500);
  worker->GetOwner()->HandleMessage(Telegram(0,0,0,Cmd_Move, &vec));
	return true;
}

bool SManufacureCom::IsSuitable(Profession* profession)
{
	return !m_bWorking;
}

void SManufacureCom::StoreExpanded()
{
  assert ("Should refactor after selling-buying system");
	//send all resources from transit store to general
	while(!m_pTransitStore->GetResourceList().empty())
	{
    m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Add(m_pTransitStore->GetResourceList().back());
		m_pTransitStore->GetResourceList().pop_back();
	}
	m_pTransitStore->UpdateAccordingToList();
	//if we sent ALL resources from transit store we begin to work
	if(0 == m_pTransitStore->GetResNumber())
	{
		m_pRequest->GetRequest().bAvailability = true;
		m_pOwner->GetOwner()->GetResourceMgr()->RefreshRequests();
		m_pOwner->StateChanged();
	}
}