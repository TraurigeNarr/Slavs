#include "Management/EconomyManagerBase.h"

#include "Management/StoreSystemBase.h"

#include "IGoldKeeper.h"

#include <algorithm>
#include <cassert>

//////////////////////////////////////////////////////////////////////////

EconomyManagerBase::EconomyManagerBase()
{

}

EconomyManagerBase::~EconomyManagerBase()
{

}

void EconomyManagerBase::Initialize()
{
  mp_store_system = std::make_shared<StoreSystemBase>();
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
      break;
      break;
    case EconomyEvent::EE_NEED_STORE:
      assert(ip_data && "ip_data should be pointer to entity that needs store.");
      mp_store_system->ProcessEvent(EconomyEvent::EE_NEED_STORE, ip_data);
      break;
    default:
      assert("Not implemented.");
      break;
    }  
  }

Slavs::TStoreSystem EconomyManagerBase::GetStoreSystem()
{
  return mp_store_system;
}