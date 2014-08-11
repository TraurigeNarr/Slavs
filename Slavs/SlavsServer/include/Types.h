#pragma once

#include <Common/Game/GameResourceTypes.h>
#include <Common/Game/GameResources.h>

#include <map>
#include <memory>
#include <vector>
#include <set>


class IEconomyManager;
class IGoldKeeper;
class IMilitaryManager;
class IRelashionshipsManager;
class IResourceManager;
class ISocietyManager;
class IStoreSystem;
class ITechnologyManager;

class EmployerInformation;
class GameResourceBox;
class Goverment;
class IController;
class SGameObject;
class SHouseComponent;
class SHumanComponent;
class SStoreHouseCom;
struct GeneralResourceInfo;

namespace Slavs
{
  class GameObject;
  class GameContext;
  class IEmployer;
  class IHouse;
  class IHuman;
  class IManufacture;
  class IStore;
  //////////////////////////////////////////////////////////////////////////
  typedef IHuman*                                 HumanPtr;
  typedef IHouse*                                 HousePtr;
  typedef IStore*                                 StorePtr;
  typedef IEmployer*                              EmployerPtr;
  typedef std::set<HumanPtr>                      Humans;
  typedef std::set<HousePtr>                      Houses;
  typedef std::vector<StorePtr>                   Stores;
  typedef std::set<EmployerPtr>                   Employers;

  typedef GameObject*                             TGameObject;
  typedef std::unique_ptr<GameContext>            TGameContext;
  typedef IEconomyManager*                        TEconomy;
  typedef IMilitaryManager*                       TMilitary;
  typedef IRelashionshipsManager*                 TRelashionships;
  typedef ISocietyManager*                        TSociety;
  typedef ITechnologyManager*                     TTechnologies;
  typedef IStoreSystem*                           TStoreSystem;
  typedef IResourceManager*                       ResourceManagerPtr;
  typedef IGoldKeeper*                            TGoldKeeper;
  typedef GameResourceBox*                        TGameResourceBox;
  typedef Goverment*                              TGoverment;
  typedef SHouseComponent*                        THouse;

  typedef std::pair<int, size_t>                  ResourcePair;
  typedef std::vector<ResourcePair>               ResourcesCountSet;

  typedef std::vector<TEconomy>                   TEconomyManagers;
  typedef std::set<TGoldKeeper>                   TGoldKeepers;
  typedef std::set<THouse>                        THouses;
  typedef std::vector<TGameResourceBox>           TResources;
  typedef std::vector<SStoreHouseCom*>            TStoreHouses;
  typedef std::vector<SHumanComponent*>           THumans;
  typedef std::vector<const EmployerInformation*> TEmployersInformation;
    typedef std::map<long, GameObject*>           TObjectsMap;
  typedef std::map<GameResourceType,GeneralResourceInfo> TReourcesInformation;

}