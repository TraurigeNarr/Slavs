#pragma once

#include "Management/IEconomyManager.h"


#include "Management/IResourceManager.h"

class EconomyManagerBase : public IEconomyManager
{
protected:
  std::unique_ptr<IStoreSystem>             mp_store_system;
  std::unique_ptr<IResourceManager>         mp_resource_manager;

public:
  virtual SLAVS_SERVER_EXPORT                           ~EconomyManagerBase();

  virtual void    SLAVS_SERVER_EXPORT                   Initialize() override;
  virtual void    SLAVS_SERVER_EXPORT                   Update(long i_elapsed_time) override;
  virtual void    SLAVS_SERVER_EXPORT                   ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) override;

  virtual Slavs::TStoreSystem                           GetStoreSystem() const override;
  virtual void                                          SetStoreSystem(std::unique_ptr<IStoreSystem>&& ip_store) override;
  virtual Slavs::ResourceManagerPtr                     GetResourceManager() const override;
  virtual void                                          SetResourceManager(std::unique_ptr<IResourceManager>&& ip_resource_manager) override;
};

//////////////////////////////////////////////////////////////////////////

inline Slavs::TStoreSystem EconomyManagerBase::GetStoreSystem() const
  {
  return mp_store_system.get();
  }

inline void EconomyManagerBase::SetStoreSystem(std::unique_ptr<IStoreSystem>&& ip_store)
  {
  mp_store_system = std::move(ip_store);
  }

inline Slavs::ResourceManagerPtr EconomyManagerBase::GetResourceManager() const
  {
  return mp_resource_manager.get();
  }

inline void EconomyManagerBase::SetResourceManager(std::unique_ptr<IResourceManager>&& ip_resource_manager)
  {
  mp_resource_manager = std::move(ip_resource_manager);
  }