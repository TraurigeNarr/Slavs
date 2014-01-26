#include "SHouseComponent.h"
//server
#include "misc/ServerEnums.h"
#include "SGameObject.h"
#include "SStaticObjCom.h"
#include "IController.h"
#include "SGameContext.h"
#include "Management/Goverment.h"
#include "Management/ISocietyManager.h"
//additional components
#include "SHumanComponent.h"
#include "SBuildingComp.h"
//common
#include <Patterns/Telegram.h>
#include <Game/GameObjectState.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <cassert>

SHouseComponent::SHouseComponent(SGameObject* owner, const TiXmlElement* componentElement)
	: m_pOwner(owner), m_iPeriod(0), m_iElapsedTicks(0), m_uiMaxNumber(0)
{
	ParseElelement(componentElement);
  owner->GetOwner()->GetGoverment().GetSocietyManager()->RegisterHouse(this);
}

SHouseComponent::~SHouseComponent()
{
	Telegram t(0,0,0,Msg_HouseDeleted);
	while(!m_lInhabitants.empty())
	{
		m_lInhabitants.back()->HandleMessage(t);
		m_lInhabitants.pop_back();
	}
  m_pOwner->GetOwner()->GetGoverment().GetSocietyManager()->UnregisterHouse(this);
}

void SHouseComponent::TickPerformed()
{
	++m_iElapsedTicks;
	//if it is not the time for new life
	if(0 == GetFreePlaces() || //if we have not enough places we can not create human
		m_iElapsedTicks < m_iPeriod)
		return;
  
	SGameObject* newHuman = m_pOwner->GetOwner()->GetContext()->AddObject(OT_Human, m_pOwner->GetComponent<SStaticObjCom>()->GetPosition()+Vector2D(rand()%100,rand()%100), 
    m_pOwner->GetOwner(), /*QM_Own*/QM_OwnButUncontroller);
	
	//move human to specific position or make him seek?
	assert(newHuman->GetComponent<SHumanComponent>() &&
		"<SHouseComponent::TickPerformed>: there is no human component");

	//add inhabitants in our house
	AddInhabitant(newHuman->GetComponent<SHumanComponent>());

	m_iElapsedTicks = 0;
}

bool SHouseComponent::HandleMessage(const Telegram& msg)
{
	return false;
}

void SHouseComponent::GetState(GameObjectState& state) const
{
  state.iFlags |= GOF_HouseInformation;

  state.uiInhabitantNumber = m_lInhabitants.size();
  state.uiInhabitantsMaxNumber = m_uiMaxNumber;
  state.uiInhabitantsWorking = 0;
  std::for_each(m_lInhabitants.begin(), m_lInhabitants.end(), [&state](SHumanComponent* ip_human)
  {
    if(nullptr != ip_human->GetCurrentProfession())
      ++state.uiInhabitantsWorking;
  });
}

size_t SHouseComponent::GetPopulation() const
{
	return m_lInhabitants.size();
}

size_t SHouseComponent::GetUnemployedPopulation() const
{
	int unemployed = 0;

	std::list<SHumanComponent*>::const_iterator iter = m_lInhabitants.begin();
	std::list<SHumanComponent*>::const_iterator end = m_lInhabitants.begin();
	for(iter; iter != end; ++iter)
	{
		if(!(*iter)->HasWork())
			++unemployed;
	}
	return unemployed;
}

void SHouseComponent::GetUnemployedPopulation(std::vector<SHumanComponent*>& unemployed) const
{
	std::list<SHumanComponent*>::const_iterator iter = m_lInhabitants.begin();
	std::list<SHumanComponent*>::const_iterator end = m_lInhabitants.end();

	for(iter; iter != end; ++iter)
	{
		if(!(*iter)->HasWork())
			unemployed.push_back((*iter));
	}
}

size_t SHouseComponent::GetFreePlaces() const
{
	return m_uiMaxNumber - m_lInhabitants.size();
}

bool SHouseComponent::AddInhabitant(SHumanComponent* inhabitant)
{
	assert(inhabitant &&
		"SHouseComponent::AddInhabitant: NULL inhabitant.");
	if(0 != GetFreePlaces())
	{
		//add inhabitant
		m_lInhabitants.push_back(inhabitant);
		inhabitant->SetHome(this);
		//report general resource controller
    m_pOwner->GetOwner()->GetGoverment().GetSocietyManager()->PopulationIncrease();
    m_pOwner->StateChanged();
		return true;
	}

	return false;
}

void SHouseComponent::RemoveInhabitant(SHumanComponent* inhabitant)
{
  m_lInhabitants.remove(inhabitant);
  m_pOwner->GetOwner()->GetGoverment().GetSocietyManager()->PopulationDecrease();
  m_pOwner->StateChanged();
}

void SHouseComponent::HumanStateChanged()
{
  m_pOwner->StateChanged();
}

void SHouseComponent::ParseElelement(const TiXmlElement* componentElement)
{
	assert(componentElement &&
		"SHouseComponent::ParseElelement: NULL parameter of config element.");
	m_uiMaxNumber = XmlUtilities::GetIntAttribute(componentElement, "population", 0);
	assert(componentElement &&
		"SHouseComponent::ParseElelement: max population == 0.");

	m_iPeriod = XmlUtilities::GetIntAttribute(componentElement, "period", 10);
}

bool SHouseComponent::Probe()
{
	//checks for SBuilding component
	assert(m_pOwner->GetComponent<SBuildingComp>() &&
		"<SStoreHouseCom::Probe>: NULL pointer of SDynamicObjCom.");
	return true;
}