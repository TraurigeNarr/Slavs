#pragma once

#include "SlavsServerAPI.h"

#include "IStoreSystem.h"

class SManufacureCom;

class SLAVS_SERVER_EXPORT StoreSystemBase : public IStoreSystem
{
protected:
  Slavs::TStoreHouses m_stores;
  //list of all resources. Need for rapid access to data about all resources that this manager is controlling.
  //all resources that are in containers are under control of the manager.
  Slavs::TReourcesInformation   m_resource_data;

  std::vector<SManufacureCom*>  m_store_waiting;
public:
  StoreSystemBase();
  virtual ~StoreSystemBase();

  virtual void          Register(SStoreHouseCom* ip_store_house) override;
  virtual void          Remove(SStoreHouseCom* ip_store_house) override;

  virtual void          Add(Slavs::TResources& i_new_resources) override;
  virtual void          Add(Slavs::TGameResourceBox i_new_resource) override;

  virtual void          Add(Slavs::ResourcePair i_resource) override;
  virtual void          Add(Slavs::ResourcesCountSet i_resources) override;

  virtual void          ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) override;
  
  virtual bool          PeekResources(GameResourceType i_type, size_t i_number) override;
  virtual size_t        GetResources(GameResourceType i_type, size_t i_number) override;
  ///returns pair where first is current number and second is max number that can be stored
  virtual TResourcePair GetResource(GameResourceType i_type) override;

  virtual Slavs::TStoreHouses& GetStores() override;
  virtual GeneralResourceInfo& GetResourceInformation(GameResourceType i_resource_type) override;
};