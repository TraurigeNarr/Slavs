#pragma once

#include "../SlavsServerAPI.h"

#include "IManager.h"

//////////////////////////////////////////////////////////////////////////

class SHouseComponent;

class SLAVS_SERVER_EXPORT ISocietyManager : public IManager
  {
  public:
    virtual ~ISocietyManager(){}

    virtual void PopulationIncrease() = 0;
    virtual void PopulationDecrease() = 0;

    virtual void RegisterHouse(Slavs::THouse ip_house) = 0;
    virtual void UnregisterHouse(Slavs::THouse ip_house) = 0;
  };