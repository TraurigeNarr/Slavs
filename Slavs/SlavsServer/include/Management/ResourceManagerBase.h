#pragma once

#include "SlavsServerAPI.h"

#include "IResourceManager.h"

class ResourceManagerBase : public IResourceManager
  {
  protected:
    Slavs::TEconomy mp_economy;

  public:
    SLAVS_SERVER_EXPORT                         ResourceManagerBase (Slavs::TEconomy ip_economy);
    virtual SLAVS_SERVER_EXPORT                 ~ResourceManagerBase();

  // IResourceManager
  public:
    virtual bool SLAVS_SERVER_EXPORT            AddResource (GameResourceBox& io_resource_box) override;
    virtual bool SLAVS_SERVER_EXPORT            AddResource (int i_type, size_t i_number) override;

    virtual size_t SLAVS_SERVER_EXPORT          PeekResource (int i_type, size_t i_number) const override;
    virtual size_t SLAVS_SERVER_EXPORT          GetResource (int i_type, size_t i_number) override;
    virtual size_t SLAVS_SERVER_EXPORT          GetResourceForcely (int i_type, size_t i_number) override;

    virtual Slavs::TEconomy                     GetEconomy() const override;
    virtual void                                SetEconomy(Slavs::TEconomy ip_economy) override;
  };

//////////////////////////////////////////////////////////////////////////

inline Slavs::TEconomy ResourceManagerBase::GetEconomy() const
  {
  return mp_economy;
  }

inline void ResourceManagerBase::SetEconomy(Slavs::TEconomy ip_economy)
  {
  mp_economy = ip_economy;
  }