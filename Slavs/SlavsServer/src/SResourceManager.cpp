#include "SResourceManager.h"

#include "SGameContext.h"
#include "ServerGameState.h"
#include "misc/ServerEnums.h"
//common
#include <Patterns/Singleton.h>
#include <Utilities/TemplateFunctions.h>
//standard
#include <algorithm>
#include <cassert>
#include <iostream>

SResourceManager::SResourceManager()
	: m_uiNeededSize(0)
{
  GiveMoney(50000);
  m_mResources[GR_Money] = new GeneralResourceInfo(GR_Money, 50000, 100000);
}

void SResourceManager::AllObjectsUpdated()
{
	Telegram t(0,0,0,Msg_AllObjectsUpdated);
	std::for_each(m_mManRequests.begin(), m_mManRequests.end(), [&t](std::pair<SManufacureCom*, ManufactureRequest*> p)
	{
		p.first->HandleMessage(t);
	});
}

SResourceManager::~SResourceManager()
{
	ClearSTLMap(m_mResources);
}

/*
void SResourceManager::Add(SStoreHouseCom* storeHouse)
{
	assert(storeHouse && 
		"<SResourceManager::AddStore>: trying to add to NULL store");
	m_vStoreHouses.push_back(storeHouse);

	const TResourceContainers& resources = storeHouse->GetResources();
	GameResourceContainer *container = NULL;
	GameResourceType grType = GR_None;
	//add resources under control
	for(size_t i = 0; i < resources.size(); ++i)
	{
		grType = resources[i]->GetResType();

		if(m_mResources.end() == m_mResources.find(grType))
		{
			m_mResources[grType] = new GeneralResourceInfo(grType, 0, 0);
		}
		m_mResources[grType]->uiMaxResources += resources[i]->GetResMaxNumber();
		m_mResources[grType]->bHasChanges = true;
	}
	//say all waiting manufactures that the store is expanded
	while(!m_lWaitingStores.empty())
	{
		m_lWaitingStores.back()->StoreExpanded();
		m_lWaitingStores.pop_back();
	}
}
*/

void SResourceManager::Add(SHouseComponent *house)
{
	assert(house && 
		"<SResourceManager::AddHouse>: trying to add to NULL house");
	m_vHouses.push_back(house);
	
	if(m_mResources.end() == m_mResources.find(GR_Population))
	{
		m_mResources[GR_Population] = new GeneralResourceInfo(GR_Population, 0, 0);
	}
	m_mResources[GR_Population]->uiMaxResources += house->GetFreePlaces();
	m_mResources[GR_Population]->bHasChanges = true;
}

/*
void SResourceManager::Add(std::list<GameResourceBox*>& newResources)
{
	while(!newResources.empty())
	{
		/ *GameResourceType grType = newResources.back()->GetGRType();
		size_t initialResources = newResources.back()->GetNumber();
		for(size_t i = 0; i < m_vStoreHouses.size(); ++i)
		{
			if(m_vStoreHouses[i]->AddResource(newResources.back()))
			{
				assert(m_mResources.end() != m_mResources.find(grType) && 
					"SResourceManager::Add[3]: there is no such container");
				m_mResources[grType]->uiCurrentResources += initialResources - newResources.back()->GetNumber();
				m_mResources[grType]->bHasChanges = true;
				initialResources = 0;
				break;//we put all resources to store
			}
		}* /
    Add(newResources.back());
		newResources.pop_back();
	}
}

size_t SResourceManager::Add(GameResourceBox* newResources)
{
	GameResourceType grType = newResources->GetGRType();
	size_t initialResources = newResources->GetNumber();
	for(size_t i = 0; i < m_vStoreHouses.size(); ++i)
	{
		m_vStoreHouses[i]->AddResource(newResources);
		if(newResources->GetNumber() == 0)
		{
			assert(m_mResources.end() != m_mResources.find(grType) && 
				"SResourceManager::Add[3]: there is no such container");
			break;//we put all resources to store
		}
	}
	m_mResources[grType]->uiCurrentResources += (initialResources - newResources->GetNumber());
	m_mResources[grType]->bHasChanges = true;
	return initialResources - newResources->GetNumber();
}*/

void SResourceManager::PopulationIncrease(int number /* = 1 */)
{
	m_mResources[GR_Population]->uiCurrentResources += number;
	m_mResources[GR_Population]->bHasChanges = true;
  assert(m_mResources[GR_Population]->uiCurrentResources >= 0);
}

void SResourceManager::PopulationDecrease(int number /* = 1 */)
{
  m_mResources[GR_Population]->uiCurrentResources -= number;
  m_mResources[GR_Population]->bHasChanges = true;
  assert(m_mResources[GR_Population]->uiCurrentResources >= 0);
}

/*
void SResourceManager::Remove(SStoreHouseCom* storeHouse)
{
	for(size_t i = 0; i < m_vStoreHouses.size(); ++i)
		if(m_vStoreHouses.at(i) == storeHouse)
			m_vStoreHouses.erase(m_vStoreHouses.begin() + i);

	//reduce the number of resources
	const  TResourceContainers& resources = storeHouse->GetResources();
	//add resources under control
	GameResourceType grType = GR_None;
	for(size_t i = 0; i < resources.size(); ++i)
	{
		grType = resources[i]->GetResType();
		m_mResources[grType]->uiMaxResources -= resources[i]->GetResMaxNumber();
		m_mResources[grType]->bHasChanges = true;
	}
}*/

void SResourceManager::Remove(SHouseComponent *house)
{
	for(size_t i = 0; i < m_vHouses.size(); ++i)
		if(m_vHouses.at(i) == house)
			m_vHouses.erase(m_vHouses.begin() + i);

	m_mResources[GR_Population]->uiMaxResources -= (house->GetPopulation() + house->GetFreePlaces());
	m_mResources[GR_Population]->bHasChanges = true;
}

bool SResourceManager::CheckAndPickResources(const std::map<GameResourceType, size_t> *const resources)
{
	std::list<GameResourceBox*> tempBag;
	bool checkRes = true;
	//check if controller has such resources
	std::for_each(resources->begin(), resources->end(), [this, &resources, &checkRes](std::pair<GameResourceType, size_t> resource)
	{
		if(m_mResources.find(resource.first) != m_mResources.end())
		{
			if(m_mResources[resource.first]->uiCurrentResources < resource.second)
			{
				checkRes = false;
				return;
			}
		}
		else
		{
			checkRes = false;
			return;
		}
	});

	if(!checkRes)
	{
		return false;
	}
	//if so, than peek this resources from stores
	size_t peekedResources = 0;
	std::for_each(resources->begin(), resources->end(), [this, &resources, &peekedResources](std::pair<GameResourceType, size_t> resource)
	{
		//resources that are needed. Decrease with each iteration of cycle. (if store house has such resource)
		//peek resource from specific store houses
		for(size_t i = 0; i < m_vStoreHouses.size(); ++i)
		{
			peekedResources += m_vStoreHouses.at(i)->PeekResources(resource.first,  resource.second - peekedResources);
			
			assert(0 <= (resource.second - peekedResources) &&
				 "<SResourceManager::CheckAndPickResource>: needed resources can not be lower zero.");

			if(peekedResources == resource.second)
				break;
		}
		//peek resource from general statistics
		m_mResources[resource.first]->uiCurrentResources -= resource.second;
		m_mResources[resource.first]->bHasChanges = true;
		peekedResources = 0;
	});

	return true;
}

bool SResourceManager::Buy(GameResourceBox* i_resource_box)
{
  assert("ResourceManager::Buy: We should delete this method from here.");
  if(GetMoney(i_resource_box->GetNumber()*i_resource_box->Price()))
  {
    m_mResources[GR_Money]->uiCurrentResources -= i_resource_box->GetNumber()*i_resource_box->Price();
    m_mResources[GR_Money]->bHasChanges = true;
    return true;
  }

  return false;
}

bool SResourceManager::Sell(IGoldKeeper* customer, GameResourceType grType, size_t number)
{
  assert("ResourceManager::Sell. We should delete this method from here.");
	if(m_mResources.find(grType) != m_mResources.end())
	{
		if(m_mResources[grType]->uiCurrentResources == 0)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	size_t peekedResources = 0;
  ResourceConfigs res_confs;
  if(!Singleton<ServerGameState>::GetInstance().GetContext().GetResConfigcs(grType,res_confs))
    return false;
  size_t price = grType*number*res_confs.m_price;
  if(!customer->GetMoney(price))
    return false;
  GiveMoney(price);
  m_mResources[GR_Money]->uiCurrentResources += price;
  m_mResources[GR_Money]->bHasChanges = true;
	for(size_t i = 0; i < m_vStoreHouses.size(); ++i)
	{
		peekedResources += m_vStoreHouses.at(i)->PeekResources(grType,  number - peekedResources);

		assert(0 <= (number - peekedResources) &&
			"<SResourceManager::CheckAndPickResource>: needed resources can not be lower zero.");

		if(peekedResources == number)
		{
			break;
		}
	}
	//peek resource from general statistics
	m_mResources[grType]->uiCurrentResources -= peekedResources;
	m_mResources[grType]->bHasChanges = true;

	return peekedResources != 0;
}

void SResourceManager::RegisterManufacture(SManufacureCom* manufacture, ManufactureRequest* request)
{
	m_mManRequests[manufacture] = request;
	//refresh request info and send it to humans
	RefreshRequests();
}

void SResourceManager::RefreshRequests()
{
	Telegram t(0,0,0, Msg_NewRequestsAvailable);
	std::for_each(m_lHumans.begin(), m_lHumans.end(), [&t](SHumanComponent* pHuman)
	{
		if(!pHuman->HasWork())
		{
			pHuman->HandleMessage(t);
		}
	});
}

ManufactureRequest const* SResourceManager::GetRequest(SManufacureCom* manufacture)
{
	std::map<SManufacureCom*, ManufactureRequest*>::const_iterator iter = m_mManRequests.find(manufacture);
	if(m_mManRequests.end() == iter)
		return NULL;
	return iter->second;
}

ManufactureRequests& SResourceManager::GetAllRequests()
{
	return m_mManRequests;
}

void SResourceManager::RegisterHuman(SHumanComponent* pHuman)
{
	m_lHumans.push_back(pHuman);
}

void SResourceManager::UnregisterHuman(SHumanComponent* p_human)
{
  m_lHumans.remove(p_human);
}

/*
void SResourceManager::ToStoreQueue(SManufacureCom* manufacture)
{
	m_lWaitingStores.push_back(manufacture);
}

void SResourceManager::ToFoodQueue(SHumanComponent* human, const std::vector<GameResourceType>& preferences)
{
	//std::find(m_lWaitingFood.begin(), m_lWaitingFood.end(), human);
}

void SResourceManager::RemoveFromQueue(SManufacureCom* manufacture)
{
	assert(m_mManRequests.end() != m_mManRequests.find(manufacture) && "<SResourceManager::RemoveFromQueue>: not registered manufacture");

	m_mManRequests.erase(m_mManRequests.find(manufacture));

	std::list<SManufacureCom*>::iterator iter;
	iter = std::find(m_lWaitingStores.begin(), m_lWaitingStores.end(), manufacture);
	if(m_lWaitingStores.end() != iter)
	{
		m_lWaitingStores.erase(iter);
	}
}

void SResourceManager::RemoveFromQueue(SHumanComponent* human)
{
	std::list<SHumanComponent*>::iterator iter = std::find(m_lWaitingFood.begin(), m_lWaitingFood.end(), human);
	if(m_lWaitingFood.end() != iter)
	{
		m_lWaitingFood.erase(iter);
	}
}*/

size_t SResourceManager::Serialize(char* buf_end, int size) const
{
	assert(0 < size &&
		"<SResourceManager::Serialize>: size is lower then zero.");
	if(m_uiNeededSize > size)
	{
		assert("<SResourceManager::Serialize>:Not enough size.");
		return 0;
	}
	if(0 == m_uiNeededSize)
		return 0;

	std::for_each(m_mResources.begin(), m_mResources.end(), [this, &buf_end, &size](std::pair<GameResourceType, GeneralResourceInfo*> container)
	{
		if(container.second->bHasChanges || container.first == GR_Money)
		{
			//size decrements with each iteration
			container.second->Serialize(buf_end, size);
			buf_end += GeneralResourceInfo::NeededSize();//increment pointer to buf_end
			size -= GeneralResourceInfo::NeededSize();//decrement size
		}
	});
	return m_uiNeededSize;
}

size_t SResourceManager::NeededSize() const
{
	m_uiNeededSize = 0;
	std::for_each(m_mResources.begin(), m_mResources.end(), [this](std::pair<GameResourceType, GeneralResourceInfo*> container)
	{
		if(container.second->bHasChanges || container.first == GR_Money)
			m_uiNeededSize += GeneralResourceInfo::NeededSize();
	});
	return m_uiNeededSize;
}