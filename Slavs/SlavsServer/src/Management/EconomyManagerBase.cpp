#include "Management/EconomyManagerBase.h"

#include "IController.h"
#include "Management/GlobalEconomics.h"
#include "Management/Goverment.h"
#include "Management/StoreSystemBase.h"
#include "Management/ResourceManagerBase.h"

#include "Game/GameContext.h"
#include "Game/GameObject.h"

#include "IGoldKeeper.h"

#include <Network/Packet.h>
#include <Network/SerializableData.h>

#include <algorithm>
#include <cassert>

//////////////////////////////////////////////////////////////////////////

EconomyManagerBase::~EconomyManagerBase()
  {

  }

void EconomyManagerBase::Initialize()
  {
  m_waiting_for_store = false;
  mp_store_system.reset(new StoreSystemBase(*this));
  mp_resource_manager.reset(new ResourceManagerBase(this));
  if (GetGoverment())
    GetGoverment()->GetOwner()->GetGameContext().GetGlobalEconomics().RegisterEconomy(this);
  }

void EconomyManagerBase::Update(long i_elapsed_time)
{

}

void EconomyManagerBase::ProcessEvent(EconomyEvent i_event, void* ip_data /* = nullptr */)
  {
  switch(i_event)
    {
    case EconomyEvent::EE_NEW_WORK_APPEARED:
      std::for_each(m_employees.begin(), m_employees.end(), [ip_data](Slavs::TGoldKeeper i_employee)
        {
        i_employee->ProcessEconomyEvent(EconomyEvent::EE_NEW_WORK_APPEARED, ip_data);
        });
      m_waiting_for_store = false;
      Invalidate();
      break;
    case EconomyEvent::EE_NEED_STORE:
      assert(ip_data && "ip_data should be pointer to entity that needs store.");
      mp_store_system->ProcessEvent(EconomyEvent::EE_NEED_STORE, ip_data);
      m_waiting_for_store = true;
      Invalidate();
      break;
    default:
      assert("Not implemented.");
      break;
    }  
  }

void EconomyManagerBase::Serialize(Network::SerializableData& i_data) const
  {
  enum class EconomyState : char
    {
    Good,Bad,Intermediate
    } economy_state;

  Slavs::TEmployersInformation info;
  GetAvailableEmployers(info);
  if (info.size() < m_employers.size())
    {
    if (m_waiting_for_store)
      economy_state = EconomyState::Bad;
    else
      economy_state = EconomyState::Good;
    }
  else
    economy_state = EconomyState::Intermediate;
  
  char state = static_cast<char>(economy_state);
  i_data.AddDataDefaultConverter<char>(0, &state, sizeof(char));

  size_t number_of_stores = mp_store_system->GetStores().size();
  i_data.AddDataDefaultConverter<size_t>(1, &number_of_stores, sizeof(size_t));
  }

void EconomyManagerBase::Deserialize(const Network::SerializableData& i_data)
  {

  }