#include "Management/EconomyManagerBase.h"

#include "Management/StoreSystemBase.h"

#include <algorithm>

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
}

Slavs::TStoreSystem EconomyManagerBase::GetStoreSystem()
{
  return mp_store_system;
}