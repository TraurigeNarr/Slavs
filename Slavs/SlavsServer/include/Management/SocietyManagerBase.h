#pragma once

#include "Management/ISocietyManager.h"

//////////////////////////////////////////////////////////////////////////

class SLAVS_SERVER_EXPORT SocietyManagerBase : public ISocietyManager
  {
  protected:
    size_t m_current_population;
    size_t m_available_places;

    Slavs::THouses m_houses;

  public:
    SocietyManagerBase();
    virtual ~SocietyManagerBase();

    virtual void PopulationIncrease() override;
    virtual void PopulationDecrease() override;

    virtual void RegisterHouse(Slavs::THouse ip_house) override;
    virtual void UnregisterHouse(Slavs::THouse ip_house) override;
  };