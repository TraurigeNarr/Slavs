#include "SHumanComponent.h"
//professions
#include "Worker.h"
//ai components
#include "AI/Goal_Think.h"
//server
#include "ISerializable.h"
#include "HumanStates.h"
#include "IController.h"
#include "SGameContext.h"
#include "SGameObject.h"
#include "ServerGameState.h"
#include "ServerUtils.h"
#include "misc\ServerEnums.h"
//additional components
#include "SBuildingComp.h"
#include "SDynamicObjCom.h"
#include "SStaticObjCom.h"
#include "SManufacureCom.h"

#include "Management/Goverment.h"
#include "Management/IEconomyManager.h"

//common
#include <Game/GameObjectState.h>
#include <Patterns/Singleton.h>
#include <Patterns/Telegram.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <cassert>
#include <iostream>
#include <memory>
//boost
#include <boost\bind.hpp>
#include <boost\function.hpp>
//constants
const size_t LowEnergy = 5;

//////////////////////////////////////////////////////////////////////////

class SHumanComponent::HumanStateWriter : public ISerializable
{
public:
  typedef std::pair<long, float> TSkillPair;
  typedef std::pair<long, size_t> TMoneyPair;
  typedef std::vector<TSkillPair> TSkillGraph;
  typedef std::vector<TMoneyPair> TMoneyGraph;
private:
  std::map<std::string, TSkillGraph> m_skill_graphs;
  TMoneyGraph m_money_graph;
  long m_human_id;
  std::wstring m_reason_to_die;
private:
  virtual std::wostream& WriteInStream(std::wostream& i_s) const override;
public:
  HumanStateWriter(long i_id);
  ~HumanStateWriter();

  void TraceMoney(long i_current_tick, size_t i_money);
  void TraceSkill(long i_current_tick, const std::string& i_profession_name, float i_skill);
  //sets alive to true
  void Dead(const std::wstring& i_reason);
};

SHumanComponent::HumanStateWriter::HumanStateWriter(long i_human_id)
  : m_human_id(i_human_id), m_reason_to_die(L"")
{

}

SHumanComponent::HumanStateWriter::~HumanStateWriter()
{
  m_money_graph.clear();
  m_skill_graphs.clear();
}

void SHumanComponent::HumanStateWriter::Dead(const std::wstring& i_reason)
{
  m_reason_to_die = i_reason;
}

std::wostream& SHumanComponent::HumanStateWriter::WriteInStream(std::wostream& i_s) const
{
  i_s << "Human: " << m_human_id << std::endl;
  if(!m_reason_to_die.empty())
    i_s << "Died because: " << m_reason_to_die.c_str() << std::endl;

  //trace profession`s skill
  std::for_each(m_skill_graphs.begin(), m_skill_graphs.end(), [&i_s](const std::pair<std::string, TSkillGraph>& i_skill_graph)
  {
    i_s << "Profession: " << i_skill_graph.first.c_str() << std::endl;
    for(size_t i = 0; i < i_skill_graph.second.size(); ++i)
    {
      i_s << i_skill_graph.second[i].first << " " << i_skill_graph.second[i].second << std::endl;
    }
    i_s << std::endl;
  });

  //trace money information
  i_s << "Money" << std::endl << "Tick money" << std::endl;
  std::for_each(m_money_graph.begin(), m_money_graph.end(), [&i_s](TMoneyPair money)
  {
    i_s << money.first << " " << money.second << std::endl;
  });
  i_s << std::endl;
  return i_s;
}

void SHumanComponent::HumanStateWriter::TraceMoney(long i_current_tick, size_t i_money)
{
  m_money_graph.push_back(TMoneyPair(i_current_tick, i_money));
}

void SHumanComponent::HumanStateWriter::TraceSkill(long i_current_tick, const std::string& i_profession_name, float i_skill)
{
  if(m_skill_graphs.end() == m_skill_graphs.find(i_profession_name))
    m_skill_graphs.emplace( std::make_pair(i_profession_name, TSkillGraph()) );

  m_skill_graphs[i_profession_name].push_back(TSkillPair(i_current_tick, i_skill));
}

//////////////////////////////////////////////////////////////////////////
SHumanComponent::SHumanComponent(SGameObject* owner, const TiXmlElement* componentElement)
	: m_pOwner(owner), m_pFSM(NULL), m_pDynamicComponent(NULL), m_hungry(false),
	m_bChangeType(false), m_pCurrentProfession(NULL), m_current_no_eaten(0), m_die_in_ticks(0)
{
	assert(componentElement);

  //initialize writer, so that human can write data from beginning
  m_state_writer = std::make_shared<HumanStateWriter>(m_pOwner->GetID());

  //set callback for money tracing
  SetCallback(boost::bind(&SHumanComponent::MoneyChanged, boost::ref(*this), _1));

	ParseElelement(componentElement);

	//register himself in central Resource manager
	m_pOwner->GetOwner()->GetResourceMgr()->RegisterHuman(this);
  m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->RegisterEmployee(Slavs::TGoldKeeper(this));
	m_pBrain = new Goal_Think(this);

  //initialize FSM and set current state to Idle
	m_pFSM = new StateMachine<SHumanComponent, long>(std::shared_ptr<SHumanComponent>(this));
	m_pFSM->SetCurrentState(Singleton<HumanIdle>::GetInstancePtr());
	//set global state
	m_pFSM->SetGlobalState(Singleton<HumanGlobal>::GetInstancePtr());

  Singleton<ServerGameState>::GetInstance().GetContext().GetOutputManager().AddItem(m_state_writer);
}

SHumanComponent::~SHumanComponent()
{
	if(NULL != m_pFSM)
		delete m_pFSM;
	if(NULL != m_pCurrentProfession)
		delete m_pCurrentProfession;
	delete m_pBrain;
  if(NULL != m_pHome)
    m_pHome->RemoveInhabitant(this);

  m_pOwner->GetOwner()->GetResourceMgr()->UnregisterHuman(this);
  m_pOwner->GetOwner()->GetGoverment().GetEconomyManager()->RemoveEmployee(Slavs::TGoldKeeper(this));
}

void SHumanComponent::TickPerformed()
{
  DecreaseEnergy();
	m_pFSM->Update(MS_FOR_TICK);
	m_pBrain->Process();
	if(NULL != m_pCurrentProfession)
		m_pCurrentProfession->TickPerformed();
}

bool SHumanComponent::HandleMessage(const Telegram& msg)
{
	bool processed = false;
	ObjectType oType = OT_None;

	processed = m_pBrain->HandleMessage(msg);
	if(processed)
		return true;

	switch(msg.Msg)
	{
	case Msg_HouseDeleted:
		{
			m_pHome = NULL;
			break;
		}
  case Msg_ObtainProfession:
    ObtainProfession(static_cast<SManufacureCom*>(msg.ExtraInfo));
    if(nullptr != m_pHome)
      m_pHome->HumanStateChanged();
    break;
	case Msg_HumanFired:
		{
			processed = true;
			if(NULL != m_pCurrentProfession && m_pCurrentProfession->IsPlaceOfWork(msg.ExtraInfo))
			{
				m_pCurrentProfession->HandleMessage(msg);
				m_pCurrentProfession = NULL;
				m_bChangeType = true;
				oType = OT_Human;
        //----------------temp
				if(NULL != m_pHome)
				{
					Vector2D vec(m_pHome->GetOwner()->GetComponent<SStaticObjCom>()->GetPosition());
          vec += Vector2D(rand()%100,rand()%100);
					m_pFSM->HandleMessage(Telegram(0,0,0,Cmd_Move, &vec));
				}
        //-----------------------
				m_pOwner->StateChanged();
        if(nullptr != m_pHome)
          m_pHome->HumanStateChanged();
			}
			break;
		}
	}//end switch

	if(!processed && m_pFSM->HandleMessage(msg))
		return true;
	//process new data with brain (can appear some changes)
	m_pBrain->Process();
	return processed;
}

void SHumanComponent::SetHome(SHouseComponent* pHome)
{
	m_pHome = pHome;
}

void SHumanComponent::GetState(GameObjectState& goState) const 
{
	if(m_bChangeType)
	{
		goState.iFlags |= GOF_ChangeType;
		m_bChangeType = false;
	}
	if(NULL != m_pCurrentProfession && m_pCurrentProfession->HasChanges())
	{
		goState.iFlags |= GOF_HasSkill;
		goState.uiSkill = m_pCurrentProfession->GetRelativeSkill();
    m_pCurrentProfession->ReleaseChanges();
	}
}

void SHumanComponent::ParseElelement(const TiXmlElement* componentElement)
{
	std::string elementName = "";

	const TiXmlElement* childElement = 0;
	const TiXmlElement*	foodElement = 0;

	while (childElement = XmlUtilities::IterateChildElements(componentElement, childElement))
	{
		elementName = childElement->Value();

		if("food" == elementName)
		{
      m_needed_calories = XmlUtilities::GetIntAttribute(childElement, "calories", 10);
      m_eat_in_ticks = XmlUtilities::GetIntAttribute(childElement, "eatInTicks", 500);
      m_die_in_ticks = XmlUtilities::GetIntAttribute(childElement, "dieInTicks", 5000);
			//parse food elements
			while (foodElement = XmlUtilities::IterateChildElements(childElement, foodElement))
			{
				elementName = foodElement->Value();

				GameResourceType grType = GameResourceBox::DefineResType(elementName);
        
        assert(grType != GR_None);

        FoodPreferences f_prefs;
        f_prefs.m_food_type = grType;
        float pref_bottom = XmlUtilities::GetRealAttribute(foodElement, "prefBottom", 0.1f);
        float pref_upper = XmlUtilities::GetRealAttribute(foodElement, "prefUpper", 1.0f);
        f_prefs.m_preference_coef = RandInRange(pref_bottom, pref_upper);
        m_food_preferences.push_back(f_prefs);
			}
		}
	}
  size_t money = XmlUtilities::GetIntAttribute(componentElement, "money",1000);
  GiveMoney(money);
}

void SHumanComponent::ObtainProfession(SManufacureCom* ip_manufacture)
{
	std::map<ObjectType, Profession*>::iterator proff = m_mProfessions.find(ip_manufacture->GetWorkerType());

	if(m_mProfessions.end() == proff)
	{
    m_mProfessions.emplace(std::make_pair(ip_manufacture->GetWorkerType(), new Worker(this)));
		proff = m_mProfessions.find(ip_manufacture->GetWorkerType());
    //set callback for profession growing
    proff->second->SetCallback( boost::bind(&SHumanComponent::SkillChanged, boost::ref(*this), _1) );
	}
	m_pCurrentProfession = proff->second;
	static_cast<Worker*>(m_pCurrentProfession)->SetPlaceOfWork(ip_manufacture);
	m_bChangeType = true;
	m_pOwner->StateChanged();
}

bool SHumanComponent::Probe()
{
	//checks for DynamicComponent (need for MoveState)
	assert(m_pOwner->GetComponent<SDynamicObjCom>() &&
		"<SHumanComponent::Probe>: NULL pointer of SDynamicObjCom.");
	m_pDynamicComponent = m_pOwner->GetComponent<SDynamicObjCom>();
	return true;
}

bool SHumanComponent::HasWork() const
{
	return NULL != m_pCurrentProfession;
}

size_t SHumanComponent::GetFood(size_t i_calories)
{
	m_calories_eaten += i_calories;
  m_hungry = m_calories_eaten < m_needed_calories ? ((m_needed_calories - m_calories_eaten) > 5) : false;
	return m_needed_calories - m_calories_eaten;
}

void SHumanComponent::DecreaseEnergy()
{
  //send message to brain first time we get hungry
  if(m_current_no_eaten == m_eat_in_ticks)
	{
    m_calories_eaten = 0;
    printf("%d need food.\n", m_pOwner->GetID());
    m_hungry = true;
		Telegram t(0,0,0, Msg_HumanHungry);
		m_pBrain->HandleMessage(t);
	}
  //update state about hungry
  if(m_current_no_eaten > m_eat_in_ticks)
  {
    if(!Hungry())
    {
      m_current_no_eaten = 0;
      m_hungry = false;

      return;
    }

    if(m_current_no_eaten - m_eat_in_ticks == m_die_in_ticks)
    {
      m_pOwner->HandleMessage(Telegram(0,0,0,Msg_HumanDied));
      m_state_writer->Dead(L"have no money for food.");
      return;
    }
  }
  ++m_current_no_eaten;
}

bool SHumanComponent::Hungry() const
{
  return m_hungry;
}

bool SHumanComponent::LeftToLife(size_t& o_time) const
{
  if(!m_hungry)
    return false;

  o_time = m_die_in_ticks > m_current_no_eaten - m_eat_in_ticks ? m_current_no_eaten - m_eat_in_ticks : 0;

  return true;
}

bool SHumanComponent::DieInTicks(size_t& o_time) const
{
  if(!m_hungry)
    return false;

  o_time = m_die_in_ticks;

  return true;
}

bool SHumanComponent::LeftToLifeRatio(float& o_ratio) const
{
  if(!m_hungry)
    return false;

  o_ratio = m_die_in_ticks > m_current_no_eaten - m_eat_in_ticks ? (m_current_no_eaten - m_eat_in_ticks)/m_die_in_ticks*1.0f : 1.0f;

  return true;
}


//////////////////////////////////////////////////////////////////////////
//callbacks

void SHumanComponent::MoneyChanged(size_t i_current_money)
{
  m_state_writer->TraceMoney(m_pOwner->GetElapsedTicksForComponents(), i_current_money);
}

void SHumanComponent::SkillChanged(float i_skill)
{
  assert(nullptr != m_pCurrentProfession);

  m_state_writer->TraceSkill(m_pOwner->GetElapsedTicksForComponents()
    , IGameObject::ToString(m_pCurrentProfession->GetProfessionType())
    , i_skill);
}