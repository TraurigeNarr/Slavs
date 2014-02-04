#include "Management/StoreSystemBase.h"

#include "SStoreHouseCom.h"
#include "SManufacureCom.h"

#include <Game/GameResources.h>

#include <algorithm>
#include <cassert>

//////////////////////////////////////////////////////////////////////////

StoreSystemBase::StoreSystemBase()
{

}

StoreSystemBase::~StoreSystemBase()
{

}

void StoreSystemBase::Register(SStoreHouseCom* ip_store_house)
{
  assert(ip_store_house && 
    "<SResourceManager::AddStore>: trying to add to NULL store");

  assert (std::find(m_stores.begin(), m_stores.end(), ip_store_house) == m_stores.end());

  m_stores.push_back(ip_store_house);

  const TResourceContainers& resources = ip_store_house->GetResources();
  GameResourceContainer *container = nullptr;
  GameResourceType res_type = GR_None;
  //add resources under control
  for(size_t i = 0; i < resources.size(); ++i)
  {
    res_type = resources[i]->GetResType();

    m_resource_data[res_type].m_res_type = res_type;
    m_resource_data[res_type].uiMaxResources += resources[i]->GetResMaxNumber();
    m_resource_data[res_type].bHasChanges = true;
  }
  //say all waiting manufactures that the store is expanded
  while(!m_store_waiting.empty())
  {
    m_store_waiting.back()->StoreExpanded();
    m_store_waiting.pop_back();
  }
}

void StoreSystemBase::Remove(SStoreHouseCom* ip_store_house)
{
  std::remove(m_stores.begin(), m_stores.end(), ip_store_house);

  //reduce the number of resources
  const  TResourceContainers& resources = ip_store_house->GetResources();
  //add resources under control
  GameResourceType res_type = GR_None;
  for(size_t i = 0; i < resources.size(); ++i)
  {
    res_type = resources[i]->GetResType();
    m_resource_data[res_type].uiMaxResources -= resources[i]->GetResMaxNumber();
    m_resource_data[res_type].bHasChanges = true;
  }
}

void StoreSystemBase::ProcessEvent(EconomyEvent i_event, void* ip_data /* = nullptr */)
{
  switch(i_event)
  {
  case EE_NEED_STORE:
    m_store_waiting.push_back(static_cast<SManufacureCom*>(ip_data));
    break;
  }
}

bool StoreSystemBase::PeekResources(GameResourceType i_type, size_t i_number) 
  {
  if (m_resource_data.find(i_type) == m_resource_data.end())
    return false;

  return m_resource_data[i_type].uiCurrentResources <= i_number;
  }

size_t StoreSystemBase::GetResources(GameResourceType i_type, size_t i_number) 
  {
  Slavs::TReourcesInformation::iterator it_res = m_resource_data.find(i_type);
  if (it_res == m_resource_data.end() || it_res->second.uiCurrentResources < i_number)
    return 0;
  it_res->second.uiCurrentResources -= i_number;
  return i_number;
  }

 IStoreSystem::TResourcePair StoreSystemBase::GetResource(GameResourceType i_type)
  {
  Slavs::TReourcesInformation::iterator it_res = m_resource_data.find(i_type);
  if (it_res == m_resource_data.end())
    return std::make_pair(0, 0);
  return std::make_pair(it_res->second.uiCurrentResources, it_res->second.uiMaxResources);
  }

void StoreSystemBase::Add(Slavs::TResources& i_new_resources)
{
  while(!i_new_resources.empty())
	{
    Add(i_new_resources.back());
		i_new_resources.pop_back();
	}
}

void StoreSystemBase::Add(Slavs::TGameResourceBox i_new_resource)
{
  GameResourceType res_type = i_new_resource->GetGRType();
  size_t initial_resources = i_new_resource->GetNumber();
  for(size_t i = 0; i < m_stores.size(); ++i)
  {
    m_stores[i]->AddResource(i_new_resource);
    if(i_new_resource->GetNumber() == 0)
    {
      assert(m_resource_data.end() != m_resource_data.find(res_type) && 
        "SResourceManager::Add[0]: there is no such container");
      break;//we put all resources to store
    }
  }
  m_resource_data[res_type].uiCurrentResources += (initial_resources - i_new_resource->GetNumber());
  m_resource_data[res_type].bHasChanges = i_new_resource->GetNumber() != initial_resources;
}

Slavs::TStoreHouses& StoreSystemBase::GetStores()
{
  return m_stores;
}

GeneralResourceInfo& StoreSystemBase::GetResourceInformation(GameResourceType i_resource_type) 
{
  assert (m_resource_data.find(i_resource_type) != m_resource_data.end() 
    && "There is no resource information. It is not critical but may cause performance drawbacks.");
  return m_resource_data[i_resource_type];
}