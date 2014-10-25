#ifndef SHouseComponent_h
#define SHouseComponent_h

#include "PluginSystem/IHouse.h"

#include "Game/IComponent.h"
#include "Game/GameResources.h"
#include <vector>

class Telegram;
class SHumanComponent;
class SGameObject;
class TiXmlElement;
class SBuildingComp;

/*
---------------------------------------------------
Implements component for increasing population.
This is houses there people are living in happiness :)
---------------------------------------------------
*/

class SHouseComponent 
  : public Slavs::IHouse
{
public:
  typedef std::list<SHumanComponent*> TInhabitants;
  //////////////////////////////////////////////////////////////////////////
	SHouseComponent(SGameObject* owner, const TiXmlElement* componentElement);
	~SHouseComponent();

	void						  TickPerformed();

	bool						  HandleMessage(const Telegram& msg);

	SGameObject*			GetOwner() const { return m_pOwner; }
	void						  GetState(GameObjectState&) const;
	//check for building component
	bool						  Probe();

	//gets number of ALL people who have already live in house
	size_t						GetPopulation() const;
	//gets number of people who has no work
	size_t						GetUnemployedPopulation() const;
	//push back into vector with unemployed workers;
	void						  GetUnemployedPopulation(std::vector<SHumanComponent*>& unemployed) const;
	//gets free places for people
	size_t						GetFreePlaces() const;
	//add inhabitant in list of house owners
	bool						  AddInhabitant(SHumanComponent* inhabitant);
  void              RemoveInhabitant(SHumanComponent* inhabitant);
  
  void              HumanStateChanged();
protected:
	//method is called with period T
	void						  IncreaseInhabitants();
	void						  ParseElelement(const TiXmlElement* componentElement);

	SGameObject*			m_pOwner;

	size_t						m_uiMaxNumber;
	TInhabitants    	m_lInhabitants;
	//period(by default is 10 ticks) for which new human created in this house until it has free places --> consider that the human is working age
	int							   m_iPeriod;
	//ticks, that pass from last period
	int							  m_iElapsedTicks;
};

#endif