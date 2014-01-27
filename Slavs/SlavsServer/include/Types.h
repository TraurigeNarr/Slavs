#pragma once

#include <Game/GameResourceTypes.h>
#include <Game/GameResources.h>

#include <map>
#include <memory>
#include <vector>
#include <set>

class EmployerInformation;
class GameResourceBox;
class GeneralResourceInfo;
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

namespace Slavs
{
  typedef std::shared_ptr<SGameObject>            TGameObject;
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

  typedef std::vector<TEconomy>                   TEconomyManagers;
  typedef std::set<TGoldKeeper>                   TGoldKeepers;
  typedef std::set<THouse>                        THouses;
  typedef std::vector<TGameResourceBox>           TResources;
  typedef std::vector<SStoreHouseCom*>            TStoreHouses;
  typedef std::vector<SHumanComponent*>           THumans;
  typedef std::vector<const EmployerInformation*> TEmployersInformation;
  typedef std::map<GameResourceType,GeneralResourceInfo> TReourcesInformation;

}