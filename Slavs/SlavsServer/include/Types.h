#pragma once

#include <Game/GameResourceTypes.h>
#include <Game/GameResources.h>

#include <map>
#include <memory>
#include <vector>


class GameResourceBox;
class GeneralResourceInfo;
class IController;
class IEconomyManager;
class IGoldKeeper;
class IMilitaryManager;
class IRelashionshipsManager;
class ISocietyManager;
class IStoreSystem;
class ITechnologyManager;
class SGameObject;
class SStoreHouseCom;

namespace Slavs
{
  typedef std::shared_ptr<SGameObject>            TGameObject;
  typedef std::shared_ptr<IEconomyManager>        TEconomy;
  typedef std::shared_ptr<IMilitaryManager>       TMilitary;
  typedef std::shared_ptr<IRelashionshipsManager> TRelashionships;
  typedef std::shared_ptr<ISocietyManager>        TSociety;
  typedef std::shared_ptr<ITechnologyManager>     TTechnologies;
  typedef std::shared_ptr<IGoldKeeper>            TGoldKeeper;
  typedef GameResourceBox*                        TGameResourceBox;
  typedef std::shared_ptr<IStoreSystem>           TStoreSystem;

  typedef std::vector<TEconomy>                   TEconomyManagers;
  typedef std::vector<TGoldKeeper>                TGoldKeepers;
  typedef std::vector<TGameResourceBox>           TResources;
  typedef std::map<GameResourceType,GeneralResourceInfo> TReourcesInformation;
  typedef std::vector<SStoreHouseCom*>            TStoreHouses;
}