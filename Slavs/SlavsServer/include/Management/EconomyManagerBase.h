#pragma once

#include "Management/IEconomyManager.h"


class SLAVS_SERVER_EXPORT EconomyManagerBase : public IEconomyManager
{
protected:
  Slavs::TStoreSystem             mp_store_system;

public:
  EconomyManagerBase();
  virtual ~EconomyManagerBase();

  virtual void    Initialize() override;
  virtual void    Update(long i_elapsed_time) override;
  virtual void    ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) override;

  virtual Slavs::TStoreSystem GetStoreSystem() override;
};