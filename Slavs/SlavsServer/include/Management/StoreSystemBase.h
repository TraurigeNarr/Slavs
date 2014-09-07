#pragma once

#include "SlavsServerAPI.h"

#include "IStoreSystem.h"

class StoreSystemBase : public IStoreSystem
{
protected:
  Slavs::Stores m_stores;
  //list of all resources. Need for rapid access to data about all resources that this manager is controlling.
  //all resources that are in containers are under control of the manager.
  Slavs::TReourcesInformation   m_resource_data;

  Slavs::Employers           m_store_waiting;
public:
  SLAVS_SERVER_EXPORT                       StoreSystemBase();
  virtual SLAVS_SERVER_EXPORT               ~StoreSystemBase();

  virtual void          SLAVS_SERVER_EXPORT Register(Slavs::StorePtr ip_store_house) override;
  virtual void          SLAVS_SERVER_EXPORT UpdateStoreInformation(Slavs::StorePtr ip_store_house) override;
  virtual void          SLAVS_SERVER_EXPORT Remove(Slavs::StorePtr ip_store_house) override;

  virtual void          SLAVS_SERVER_EXPORT Add(Slavs::TResources& io_new_resources) override;
  virtual void          SLAVS_SERVER_EXPORT Add(Slavs::TGameResourceBox iop_new_resource) override;

  virtual void          SLAVS_SERVER_EXPORT Add(Slavs::ResourcePair& io_resource) override;
  virtual void          SLAVS_SERVER_EXPORT Add(Slavs::ResourcesCountSet& io_resources) override;

  virtual void          SLAVS_SERVER_EXPORT ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) override;
  
  virtual bool          SLAVS_SERVER_EXPORT PeekResources(GameResourceType i_type, size_t i_number) override;
  virtual size_t        SLAVS_SERVER_EXPORT GetResources(GameResourceType i_type, size_t i_number) override;
  ///returns pair where first is current number and second is max number that can be stored
  virtual TResourcePair SLAVS_SERVER_EXPORT GetResource(GameResourceType i_type) override;

  virtual SLAVS_SERVER_EXPORT Slavs::Stores&        GetStores() override;
  virtual SLAVS_SERVER_EXPORT GeneralResourceInfo&  GetResourceInformation(GameResourceType i_resource_type) override;
};