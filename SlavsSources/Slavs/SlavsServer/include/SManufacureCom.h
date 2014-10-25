#ifndef SManufacureCom_h
#define SManufacureCom_h

#include "ManufactureRequest.h"
#include "IGoldKeeper.h"

#include "PluginSystem/IEmployer.h"

#include <Game/Enumerations.h>
#include <Game/GameResources.h>
#include <Game/IComponent.h>

#include <map>
#include <vector>

//////////////////////////////////////////////////////////////////////////

class Telegram;
class SGameObject;
class TiXmlElement;
class SBuildingComp;
class SHumanComponent;
class Profession;

//////////////////////////////////////////////////////////////////////////

/*
---------------------------------------------------
Component describes some productive industry houses.
This can be sawmill, quarry, etc.
---------------------------------------------------
*/
class SManufacureCom 
  : public Slavs::IEmployer
{
public:
									  SManufacureCom(SGameObject* owner, const TiXmlElement* componentElement);
									  virtual ~SManufacureCom();

	void							TickPerformed();
	bool							HandleMessage(const Telegram& msg);

	SGameObject*			GetOwner() const { return m_pOwner; }
	void							GetState(GameObjectState&) const;
	//check for building component
	bool							Probe();

  virtual void ProcessEconomyEvent(EconomyEvent i_event, void* ip_data = nullptr) override;

	/************************************************************************/
	/*                      Getters/Setters                                 */
	/************************************************************************/
	GameResourceType	GetGRType() const;
	int								GetGrowthPeriod() const { return m_iGrowthPeriod; }
	int       				GetWorkerType() const { return m_WorkerType; }
	float							GetInitialSkill() const { return m_fInitialSkill; }
  float             GetSkillGrowth() const { return m_fSkillGrowth; }
  float             GetMaxSkill() const { return m_fMaxSkill; }
  float             GetLearningCoefficient() const { return m_learning_coefficient; }
	int								GetIniResMines() const { return m_iIniResMines; }
	bool						  NeedWorkers() const { return 0 == m_iNeededWorkers; }
  inline const EmployerInformation& GetInformation() const;
	//calls from ResourceManager if there is a population. Returns true if hired, false otherwise
	//@param worker - human that can be hired. if he will - moves to position of factory(temporary)
	bool							HireWorker(SHumanComponent* worker);
	//returns true, if this human`s profession information(e.x. skill) is suitable for this manufacture requirements
	//@param profession - human`s profession information, NULL if human has no profession of this type
	//returns only true for now
	bool							IsSuitable(Profession* profession);
	//calls from ResourceManager if there is a store appeared
	void							StoreExpanded();

protected:
	void							ParseElelement(const TiXmlElement* componentElement);

	SGameObject*					m_pOwner;
/* Specific information */
	std::shared_ptr<GameResourceContainer>			m_pTransitStore;
	GameResourceBox*				m_pBoxOfResources;
	//Resource that is mining
	GameResourceType				m_grType;
	//initial skill of worker (just come to the production)
	float							m_fInitialSkill;
	//number of resource that human mines at first
	int								m_iIniResMines;
	//period for that worker gets skill growth - in cycles
	int								m_iGrowthPeriod;
	//operating cycle in ticks
	int								m_iOperatingCycle;
	int								m_iCurrentTick;
	bool							m_bWorking;
	//number of workers that already passed resources and waiting for new cycle
	int								m_iWorkersEndsCycle;
	//learning curve coefficient
	float							m_learning_coefficient;
  //max skill
  float             m_fMaxSkill;
  //skill increase coefficient (per one period)
	float							m_fSkillGrowth;
	//needed number of workers - assume, that needed number = max number
	int								m_iNeededWorkers;
	int   						m_WorkerType;
	//request that is holden in Resource manager
	EmployerInformation*			m_pRequest;
	//payment
	size_t							m_uiPayment;
	//list of workers
	std::vector<SHumanComponent*>	m_vWorkers;

  enum ManufactureState
    {
    MS_WAITING_FOR_WORKERS,
    MS_WAITING_FOR_STORES,
    MS_PRE_WORKING,
    MS_WORKING,
    };
  ManufactureState m_state;
};

const EmployerInformation& SManufacureCom::GetInformation() const
  {
  return *m_pRequest;
  }

#endif