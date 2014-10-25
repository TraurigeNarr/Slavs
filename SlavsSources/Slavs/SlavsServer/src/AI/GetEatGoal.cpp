#include "AI/GetEatGoal.h"
//server
#include "HumanStates.h"
#include "IController.h"
#include "SGameContext.h"
#include "SGameObject.h"
#include "SResourceManager.h"
#include "SResourceManager.h"
#include "SStaticObjCom.h"

#include <Patterns/Singleton.h>
#include <Patterns/Telegram.h>

GetEatGoal::GetEatGoal(SHumanComponent* pOwner)
	: Goal<SHumanComponent>(pOwner, GT_Eat)
{}

GetEatGoal::~GetEatGoal()
{}

void GetEatGoal::Activate()
{
	m_gStatus = active;
  Vector2D vec(m_pOwner->GetHome()->GetOwner()->GetComponent<SStaticObjCom>()->GetPosition());
  if(m_pOwner->GetDynamicComponent()->GetPosition().DistanceSq(vec) > 200 && !m_pOwner->GetFSM()->IsInState(Singleton<HumanMove>::GetInstancePtr()))
    m_pOwner->GetFSM()->HandleMessage(Telegram(0,0,0,Cmd_Move, &vec));
}

int GetEatGoal::Process()
{
  if(m_pOwner->GetFSM()->IsInState(Singleton<HumanMove>::GetInstancePtr()))
	  m_gStatus = active;
  else
  {
    GetFood();
    m_gStatus = completed;
  }
	return m_gStatus;
}

void GetEatGoal::Terminate()
{
}

void GetEatGoal::GetFood()
{
  const std::vector<FoodPreferences>& preferences = m_pOwner->GetFoodPreferences();
  ResourceConfigs res_configs;
  while(m_pOwner->Hungry())
  {
    size_t best_result_index = 0;
    double best_result_value = 0.0;

    //calculate best choice
    for(size_t i = 0; i < preferences.size(); ++i)
    {
      
      assert(m_pOwner->GetOwner()->GetContext()->GetResConfigcs(preferences[i].m_food_type, res_configs));
      m_pOwner->GetOwner()->GetContext()->GetResConfigcs(preferences[i].m_food_type, res_configs);
      
      double calories = res_configs.m_calories/m_pOwner->GetNeededCalories();
      double price = (m_pOwner->HasMoney()*1.0)/res_configs.m_price;//to zero if we have money, greater then 1 if have not
      double cur_result = preferences[i].m_preference_coef*(calories < 1.0 ? calories : 1.0)*(price < 1.0 ? price : 1.0);

      //for getting what we want
      if(best_result_value < cur_result)
      {
        best_result_value = cur_result;
        best_result_index = i;
      }
    }
    //if we have no money to buy, break from cycle
    if(best_result_value > 0.0 && //we have no money
      false != m_pOwner->GetOwner()->GetOwner()->GetResourceMgr()->Sell(m_pOwner, preferences[best_result_index].m_food_type, 1))
    {
        m_pOwner->GetOwner()->GetContext()->GetResConfigcs(preferences[best_result_index].m_food_type, res_configs);
        m_pOwner->GetFood(res_configs.m_calories);
        printf("%d get some food(%s-%d)!\n", m_pOwner->GetOwner()->GetID(), GameResourceBox::GetResTypeString(res_configs.m_type).c_str(),res_configs.m_calories);
    }
    else
    {
      printf("%d has no money(%d)\n", m_pOwner->GetOwner()->GetID(), m_pOwner->HasMoney());
      break;
    }
  }
}