#pragma once

#include "Management/ISocietyManager.h"

//////////////////////////////////////////////////////////////////////////

class SocietyManagerBase : public ISocietyManager
  {
  protected:
    size_t m_current_population;
    size_t m_available_places;

    Slavs::Houses m_houses;

  public:
    SLAVS_SERVER_EXPORT SocietyManagerBase();
    virtual SLAVS_SERVER_EXPORT ~SocietyManagerBase();

    virtual SLAVS_SERVER_EXPORT void PopulationIncrease() override;
    virtual SLAVS_SERVER_EXPORT void PopulationDecrease() override;

    virtual SLAVS_SERVER_EXPORT void RegisterHouse(Slavs::HousePtr ip_house) override;
    virtual SLAVS_SERVER_EXPORT void UnregisterHouse(Slavs::HousePtr ip_house) override;
  };