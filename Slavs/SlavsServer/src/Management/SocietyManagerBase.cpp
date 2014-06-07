#include "Management/SocietyManagerBase.h"

#include "PluginSystem/IHouse.h"

#include <cassert>

//////////////////////////////////////////////////////////////////////////

SocietyManagerBase::SocietyManagerBase()
  : m_available_places(0)
  , m_current_population(0)
  {  }

SocietyManagerBase::~SocietyManagerBase()
  {  }

void SocietyManagerBase::PopulationIncrease()
  {
  assert (m_current_population + 1 <= m_available_places);
  ++m_current_population;
  _Invalidate();
  }

void SocietyManagerBase::PopulationDecrease()
  {
  assert(m_current_population > 1);
  --m_current_population;
  _Invalidate();
  }

void SocietyManagerBase::RegisterHouse(Slavs::HousePtr ip_house)
  {  
  if (m_houses.insert(ip_house).second)
    {
    m_available_places += ip_house->GetFreePlaces();
    _Invalidate();
    }
  }

void SocietyManagerBase::UnregisterHouse(Slavs::HousePtr ip_house)
  {
  size_t number_of_erased = m_houses.erase(ip_house);
  assert(number_of_erased == 1);
  
  if (number_of_erased == 1)
    {
    m_available_places -= (ip_house->GetFreePlaces()+ip_house->GetPopulation());
    _Invalidate();
    }
  }