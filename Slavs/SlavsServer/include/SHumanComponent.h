#ifndef SHumanComponent_h
#define SHumanComponent_h

#include "IGoldKeeper.h"
#include "Profession.h"
#include "SDynamicObjCom.h"

#include <Game/GameResources.h>
#include <Game/IComponent.h>
#include <Game/RequestForWorkers.h>
#include <Patterns/StateMachine.h>

#include <list>
#include <map>
#include <memory>

//////////////////////////////////////////////////////////////////////////
class Telegram;
class SGameObject;
class SHouseComponent;
class TiXmlElement;

class SManufacureCom;

class Goal_Think;
//////////////////////////////////////////////////////////////////////////
const size_t MaxEnergy = 100;

struct FoodInformation
{
	GameResourceType	grFood;
	size_t				uiNumber;
};

typedef std::map<ObjectType, Profession*> HumanProfessions;

struct FoodPreferences
{
GameResourceType m_food_type;
float m_preference_coef;
};

/*
---------------------------------------------------
This is the base component for humans. Considered that
the human of working age so that he can obtain some profession
like farmer, lumberjack, warrior, etc.
The profession will be selected after creating of human object
then one of profession will be needed by some factory/army.
Human has it`s own FSM (basic states: Move, Idle) that extends with profession states.
---------------------------------------------------
*/
class SHumanComponent : public IComponent, 
                        public IGoldKeeper
{
public:
										              SHumanComponent(SGameObject* owner, const TiXmlElement* componentElement);
										              ~SHumanComponent();
	//virtual overrides
	void								            TickPerformed();
	bool								            HandleMessage(const Telegram& msg);
	void								            SetHome(SHouseComponent* pHome);

	SGameObject*						        GetOwner() const { return m_pOwner; }
	void								            GetState(GameObjectState&) const;
	bool								            Probe();

  virtual void ProcessEconomyEvent(EconomyEvent i_event, void* ip_data = nullptr) override;
	//getters
	StateMachine<SHumanComponent, long>*  GetFSM() const { return m_pFSM; }
  SHouseComponent*                GetHome() const {return m_pHome;}
	Goal_Think*							        GetBrain() const { return m_pBrain; }
	const HumanProfessions&				  GetProfessions() const { return m_mProfessions; }
	Profession*							        GetCurrentProfession() const { return m_pCurrentProfession; }
	SDynamicObjCom*						      GetDynamicComponent() const { return m_pDynamicComponent; }
	//accessories
	bool								            HasWork() const;
	void								            DecreaseEnergy();


	//eat the food and returns food, that the human needs
	size_t								          GetFood(size_t i_calories);
  const std::vector<FoodPreferences>& GetFoodPreferences() const { return m_food_preferences; }
  size_t                          GetNeededCalories() const { return m_needed_calories - m_calories_eaten; }
  bool                            Hungry() const;
  //how many ticks will human live, if he is hungry
  bool                          LeftToLife(size_t& o_time) const;
  bool                          DieInTicks(size_t& o_time) const;
  //ratio between Time without eat and max time for THIS human without eat
  bool                           LeftToLifeRatio(float& o_ratio) const;

private:
	void								            ParseElelement(const TiXmlElement* componentElement);
  void								            ObtainProfession(SManufacureCom* ip_manufacture);

  void                            SkillChanged(float i_skill);
  void                            MoneyChanged(size_t i_current_money);


  /*
  this class destination is to keep data about human
  and write it to ostream. Overloads of << operator
  */
  class HumanStateWriter;

  std::shared_ptr<HumanStateWriter> m_state_writer;
  mutable bool						          m_bChangeType;
	StateMachine<SHumanComponent, long>*		m_pFSM;
  std::vector<FoodPreferences>      m_food_preferences;
	Goal_Think*							          m_pBrain;
	//references to "world"
	SGameObject*						          m_pOwner;
	SDynamicObjCom*						        m_pDynamicComponent;
	SHouseComponent*					        m_pHome;

	Profession*							          m_pCurrentProfession;
	//professions that the human already had
	HumanProfessions					        m_mProfessions;

	//human wants to eat after that tick
	size_t								            m_eat_in_ticks;
  //tick from last eaten
  size_t                            m_current_no_eaten;
  size_t                            m_needed_calories;
  //
  size_t                            m_die_in_ticks;
  //that eaten, must be zero then begin eating
  size_t                            m_calories_eaten;

  bool                              m_hungry;
};

#endif