#ifndef SResourceManager_h
#define SResourceManager_h

#include "SStoreHouseCom.h"
#include "SHouseComponent.h"
#include "SManufacureCom.h"
#include "SHumanComponent.h"
#include "ManufactureRequest.h"
#include "IGoldKeeper.h"

#include "Game/GameResources.h"
#include <vector>
#include <list>
#include <map>

/*
--------------------------------------------------------------------
This class keeps the list of all store houses.
Then some needs resource Mgr asks for resource from stores.
To add resource Mgr must find store with empty place (recursively)
and places parts of resource in stores.
@potentially to implement different strategies of adding/removing resources -> queue
--------------------------------------------------------------------
*/

class SResourceManager : public IGoldKeeper
{
	typedef std::map<GameResourceType,GeneralResourceInfo*> ReourceContainers;
public:
	SResourceManager();
	~SResourceManager();

	void									AllObjectsUpdated();

	/*void									Add(SStoreHouseCom* storeHouse);*/
	void									Add(SHouseComponent *house);
	//returns the number of resource that added. The resource can be added partially if there is not enough space in store houses
	/*void									Add(std::list<GameResourceBox*>& newResources);
	size_t									Add(GameResourceBox* newResources);
	//this method is calling from houses than human created*/
	//@param number - number of new humans
	void									PopulationIncrease(int number = 1);
  void                  PopulationDecrease(int number = 1);

	/*void									Remove(SStoreHouseCom* storeHouse);*/
	void									Remove(SHouseComponent* house);

	//@param resources - list of needed resources
	bool									CheckAndPickResources(const std::map<GameResourceType, size_t> *const resources);
	//peek as many resources as there are and returns number of peeked resources
	bool                  Sell(IGoldKeeper* customer, GameResourceType grType, size_t number);
  bool                  Buy(GameResourceBox* i_resource_box);
	//register manufacture`s request
	void									RegisterManufacture(SManufacureCom* manufacture, ManufactureRequest* request);
	//manufacture reports manager that request has been refreshed so that it(manager) can tell humans
	void									RefreshRequests();
	ManufactureRequest const*		GetRequest(SManufacureCom* manufacture);
	ManufactureRequests&					GetAllRequests();
	//register human in manager, so that he will be processing for requests
	void									RegisterHuman(SHumanComponent* pHuman);
  void                  UnregisterHuman(SHumanComponent* p_human);

	//call from SManufacture --> push into queue also calls then the stores is expanded
	/*void									ToStoreQueue(SManufacureCom* manufacture);*/
	//human gets to this queue then he needs to restore energy
	//@param human - pointer to human component
	//@param preferences - does not use now
	/*void									ToFoodQueue(SHumanComponent* human, const std::vector<GameResourceType>& preferences);*/

	/*void									RemoveFromQueue(SManufacureCom* manufacture);
	void									RemoveFromQueue(SHumanComponent* human);*/

	/*void									GetOutOfQueue(SManufacureCom* human);
	void									GetOutOfQueue(SHumanComponent* human);*/

	size_t									Serialize(char* buf_end, int size) const;
	size_t									NeededSize() const;
private:
	//list of all store houses. Added here material resources.
	std::vector<SStoreHouseCom*>			m_vStoreHouses;
	//list of houses, there people are living
	std::vector<SHouseComponent*>			m_vHouses;
	//list of all resources. Need for rapid access to data about all resources that this manager is controlling.
	//all resources that are in containers are under control of the manager.
	ReourceContainers						m_mResources;
	//needed size for current serialization. Depends on containers that changed their state.
	mutable size_t							m_uiNeededSize;

	//list of requests from manufactures. Then manufacture is created it must get to this list
	ManufactureRequests						m_mManRequests;
	std::list<SHumanComponent*>				m_lHumans;

	//list of manufactures that needs workers
	std::list<SManufacureCom*>				m_lWaitingStores;
	//list of humans that need food
	std::list<SHumanComponent*>				m_lWaitingFood;
	//list of people who has no home for know (if the home was destroyed)
	std::list<SHumanComponent*>				m_lHomeless;
};

#endif