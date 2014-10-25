#include "Management/StoreSystemBase.h"

#include "Management/IEconomyManager.h"

#include "PluginSystem/IStore.h"
#include "PluginSystem/IEmployer.h"

#include <Game/GameResources.h>
#include <Game/GameResource.h>

#include <algorithm>
#include <cassert>

//////////////////////////////////////////////////////////////////////////

StoreSystemBase::StoreSystemBase(IEconomyManager& i_manager)
  : IStoreSystem(i_manager)
  {  }

StoreSystemBase::~StoreSystemBase()
{

}

void StoreSystemBase::Register(Slavs::StorePtr ip_store_house)
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

  //say all waiting employers that the store is expanded
  for (auto p_store_waiting : m_store_waiting)
    p_store_waiting->StoreExpanded();
  m_store_waiting.clear();

  m_manager.Invalidate();
}

void StoreSystemBase::UpdateStoreInformation(Slavs::StorePtr ip_store_house)
  {
  assert(ip_store_house && 
    "<StoreSystemBase::UpdateStoreInformation>: trying to add to NULL store");

  auto store_it = std::find(m_stores.begin(), m_stores.end(), ip_store_house);

  assert (store_it != m_stores.end() &&
    "<StoreSystemBase::UpdateStoreInformation>: trying to update store which was not registered");

  throw std::logic_error("not implemented");
  }

void StoreSystemBase::Remove(Slavs::StorePtr ip_store_house)
{
  auto new_end_it = std::remove(m_stores.begin(), m_stores.end(), ip_store_house);
  m_stores.erase(new_end_it, m_stores.end());

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

  m_manager.Invalidate();
}

void StoreSystemBase::ProcessEvent(EconomyEvent i_event, void* ip_data /* = nullptr */)
{
  switch(i_event)
    {
    case EconomyEvent::EE_NEED_STORE:
      m_store_waiting.insert(static_cast<Slavs::EmployerPtr>(ip_data));
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

  size_t resources_left = i_number;
  for (Slavs::StorePtr ip_store : m_stores)
    {
    resources_left -= ip_store->GetResourceForcely(i_type, resources_left);
    if (resources_left == 0)
      break;
    }

  it_res->second.uiCurrentResources -= i_number;
  m_manager.Invalidate();
  return i_number;
  }

 IStoreSystem::TResourcePair StoreSystemBase::GetResource(GameResourceType i_type)
  {
  Slavs::TReourcesInformation::iterator it_res = m_resource_data.find(i_type);
  if (it_res == m_resource_data.end())
    return std::make_pair(0, 0);
  return std::make_pair(it_res->second.uiCurrentResources, it_res->second.uiMaxResources);
  }

void StoreSystemBase::Add(Slavs::TResources& io_new_resources)
{
  while(!io_new_resources.empty())
	{
    Add(io_new_resources.back());
    if (io_new_resources.back()->Empty())
      io_new_resources.pop_back();
	}
  m_manager.Invalidate();
}

void StoreSystemBase::Add(Slavs::TGameResourceBox iop_new_resource)
{
  GameResourceType res_type = iop_new_resource->GetGRType();
  size_t initial_resources = iop_new_resource->GetNumber();

  for(size_t i = 0; i < m_stores.size(); ++i)
  {
    m_stores[i]->AddResource(*iop_new_resource);
    if(iop_new_resource->GetNumber() == 0)
    {
      assert(m_resource_data.end() != m_resource_data.find(res_type) && 
        "SResourceManager::Add[0]: there is no such container");
      break;//we put all resources to store
    }
  }
  m_resource_data[res_type].uiCurrentResources += (initial_resources - iop_new_resource->GetNumber());
  m_resource_data[res_type].bHasChanges = iop_new_resource->GetNumber() != initial_resources;
  m_manager.Invalidate();
}

void StoreSystemBase::Add(Slavs::ResourcePair& io_resource)
  {
  GameResourceType res_type = static_cast<GameResourceType>(io_resource.first);
  size_t initial_resources = io_resource.second;
  
  GameResourceBox box(res_type, io_resource.second, 0, 0);
  Add(&box);
  io_resource.second = box.GetNumber();
  m_manager.Invalidate();
  }

void StoreSystemBase::Add(Slavs::ResourcesCountSet& io_resources)
  {
  for (Slavs::ResourcePair& resource : io_resources)
    Add(resource);
  m_manager.Invalidate();
  }

Slavs::Stores& StoreSystemBase::GetStores()
{
  return m_stores;
}

GeneralResourceInfo& StoreSystemBase::GetResourceInformation(GameResourceType i_resource_type) 
{
  assert (m_resource_data.find(i_resource_type) != m_resource_data.end() 
    && "There is no resource information. It is not critical but may cause performance drawbacks.");
  return m_resource_data[i_resource_type];
}