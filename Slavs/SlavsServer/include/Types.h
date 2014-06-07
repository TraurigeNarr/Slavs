#pragma once

#include <Game/GameResourceTypes.h>
#include <Game/GameResources.h>

#include <map>
#include <memory>
#include <vector>
#include <set>

class EmployerInformation;
class GameResourceBox;
class Goverment;
class IController;
class IEconomyManager;
class IGoldKeeper;
class IMilitaryManager;
class IRelashionshipsManager;
class ISocietyManager;
class IStoreSystem;
class ITechnologyManager;
class SGameObject;
class SHouseComponent;
class SHumanComponent;
class SStoreHouseCom;
struct GeneralResourceInfo;

namespace Slavs
{
  class GameObject;
  class GameContext;
  class IHouse;
  class IHuman;
  class IManufacture;
  class IStore;
  class IEmployer;
  //////////////////////////////////////////////////////////////////////////
  typedef IHuman*                                 HumanPtr;
  typedef IHouse*                                 HousePtr;
  typedef IStore*                                 StorePtr;
  typedef IEmployer*                              EmployerPtr;
  typedef std::set<HumanPtr>                      Humans;
  typedef std::set<HousePtr>                      Houses;
  typedef std::vector<StorePtr>                   Stores;
  typedef std::vector<EmployerPtr>                Employers;

  typedef GameObject*                             TGameObject;
  typedef std::unique_ptr<GameContext>            TGameContext;
  typedef std::shared_ptr<IEconomyManager>        TEconomy;
  typedef std::shared_ptr<IMilitaryManager>       TMilitary;
  typedef std::shared_ptr<IRelashionshipsManager> TRelashionships;
  typedef std::shared_ptr<ISocietyManager>        TSociety;
  typedef std::shared_ptr<ITechnologyManager>     TTechnologies;
  typedef std::shared_ptr<IStoreSystem>           TStoreSystem;
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