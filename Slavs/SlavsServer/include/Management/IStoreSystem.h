#pragma once

#include "SlavsServerAPI.h"

#include "EventsEnumerations.h"
#include "Types.h"

class SStoreHouseCom;

class SLAVS_SERVER_EXPORT IStoreSystem
{
public:
  typedef std::pair<size_t, size_t> TResourcePair;

public:
  virtual ~IStoreSystem(){}

  virtual void            Register(SStoreHouseCom* ip_store_house) = 0;
  virtual void            Remove(SStoreHouseCom* ip_store_house) = 0;
  
  virtual void            Add(Slavs::TResources& i_new_resources) = 0;
  virtual void            Add(Slavs::TGameResourceBox i_new_resource) = 0;

  virtual void            Add(Slavs::ResourcePair i_resource) = 0;
  virtual void            Add(Slavs::ResourcesCountSet i_resources) = 0;

  virtual void            ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) = 0;

  virtual bool            PeekResources(GameResourceType i_type, size_t i_number) = 0;
  virtual size_t          GetResources(GameResourceType i_type, size_t i_number) = 0;
  virtual TResourcePair   GetResource(GameResourceType i_type) = 0;

  virtual Slavs::TStoreHouses& GetStores() = 0;
  virtual GeneralResourceInfo& GetResourceInformation(GameResourceType i_resource_type) = 0;
};