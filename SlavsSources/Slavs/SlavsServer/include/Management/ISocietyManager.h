#pragma once

#include "IManager.h"

//////////////////////////////////////////////////////////////////////////

class ISocietyManager : public IManager
  {
  public:
    virtual ~ISocietyManager(){}

    virtual void PopulationIncrease() = 0;
    virtual void PopulationDecrease() = 0;

    virtual void RegisterHouse(Slavs::HousePtr ip_house) = 0;
    virtual void UnregisterHouse(Slavs::HousePtr ip_house) = 0;
  };